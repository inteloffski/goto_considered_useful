#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

typedef struct {
  char *memory;
  size_t size;
} Buffer;

typedef struct {
  const char *direction;
  const char *name_ru;
} WindDirection;

const WindDirection wind_directions[] = {
  {"N",   "Север"},
  {"NNE", "Северо-северо-восток"},
  {"NE",  "Северо-восток"},
  {"ENE", "Востоко-северо-восток"},
  {"E",   "Восток"},
  {"ESE", "Востоко-юго-восток"},
  {"SE",  "Юго-восток"},
  {"SSE", "Юго-юго-восток"},
  {"S",   "Юг"},
  {"SSW", "Юго-юго-запад"},
  {"SW",  "Юго-запад"},
  {"WSW", "Западо-юго-запад"},
  {"W",   "Запад"},
  {"WNW", "Западо-северо-запад"},
  {"NW",  "Северо-запад"},
  {"NNW", "Северо-северо-запад"}
};

const char* direction_name(const char *direction) 
{
  size_t count = sizeof(wind_directions) / sizeof(wind_directions[0]);
  for (size_t i = 0; i < count; ++i) {
    if(strcmp(wind_directions[i].direction, direction) == 0) {
      return wind_directions[i].name_ru; 
    }
  }
  return "Неизвестное направление";
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void * userp)
{
  size_t realsize = size * nmemb;
  Buffer *mem = (Buffer *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory */
    printf("not enugh memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

void weather_print(
    const char *city,
    cJSON *temp_c, 
    cJSON *feels_like_c, 
    cJSON *humidity, 
    cJSON *windDir,
    cJSON *windSpeed,
    cJSON *descWeather)
{
  if (
      cJSON_IsString(temp_c) && 
      cJSON_IsString(feels_like_c) && 
      cJSON_IsString(humidity) &&
      cJSON_IsString(windDir) &&
      cJSON_IsString(windSpeed) &&
      cJSON_IsString(descWeather)
     ) { 
    printf("| %-s: %-s \n", "Город", city);
    printf("| %-s: %-s (°C)\n", "Температура", temp_c ->valuestring);
    printf("| %-s: %-s (°C)\n", "Ощущается как", feels_like_c->valuestring);
    printf("| %-s: %-s (%%)\n", "Влажность", humidity->valuestring);
    printf("| %-s: %-s \n", "Направление ветра", direction_name(windDir->valuestring));
    printf("| %-s: %-s (км/ч)\n", "Скорость ветра", windSpeed->valuestring);
    printf("| %-s: %-s \n", "Описание погоды", descWeather->valuestring);
  }
}

void parsing_success_response(Buffer buffer, const char *city)
{
  cJSON *json = cJSON_Parse(buffer.memory);
  if(json == NULL) {
    fprintf(stderr,"Error parsing JSON\n");
  }
  // обработка массива "current_condition"
  cJSON *current_array = cJSON_GetObjectItem(json, "current_condition");
  if (current_array && cJSON_IsArray(current_array)) {
    cJSON *current = cJSON_GetArrayItem(current_array, 0);
    if (current) {
      cJSON *humidity = cJSON_GetObjectItem(current, "humidity");
      cJSON *temp_c = cJSON_GetObjectItem(current, "temp_C");
      cJSON *feels_like_c = cJSON_GetObjectItem(current, "FeelsLikeC"); 
      cJSON *wind_dir = cJSON_GetObjectItem(current, "winddir16Point");
      cJSON *wind_speed = cJSON_GetObjectItem(current, "windspeedKmph");

      cJSON *lang_ru_array = cJSON_GetObjectItem(current, "lang_ru");

      if (lang_ru_array && cJSON_IsArray(lang_ru_array)) {
        cJSON *lang_ru_item = cJSON_GetArrayItem(lang_ru_array, 0);
        if(lang_ru_item) {
          cJSON *description_weather = cJSON_GetObjectItem(lang_ru_item, "value");

          weather_print(
              city,
              temp_c, 
              feels_like_c, 
              humidity, 
              wind_dir, 
              wind_speed,
              description_weather
              );
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  // проверка на количество переданных аргументов
  if(argc != 2) {
    fprintf(stderr, "Usage: %s <CityName>\n", argv[0]);
    return EXIT_FAILURE;
  }
  setlocale(LC_ALL, "");
  ////////////////////////////////////////////////
  // Блок кода для инициализации базовых сущностей
  ////////////////////////////////////////////////
  // аргумент города
  const char *city = argv[1];
  // инициализация библиотеки libcurl 
  curl_global_init(CURL_GLOBAL_ALL);
  // инициализируем структуру CURL
  CURL *curl = curl_easy_init();
  // инициализируем буфер 
  Buffer buffer;

  buffer.memory = malloc(1);
  buffer.size = 0;

  if(curl) {
    char url[512];
    snprintf(url, sizeof(url), "https://wttr.in/%s?format=j1&lang=ru", city);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&buffer);

    CURLcode res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perfom() failed: %s\n", curl_easy_strerror(res));
    } else {
      parsing_success_response(buffer, city);      
    }

    curl_easy_cleanup(curl);
    free(buffer.memory);
  }

  curl_global_cleanup();
  return EXIT_SUCCESS;
}
