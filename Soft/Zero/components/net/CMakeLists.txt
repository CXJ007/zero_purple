idf_component_register(SRCS "nets.c"
                       INCLUDE_DIRS "."
                       REQUIRES nvs_flash esp_http_client json)