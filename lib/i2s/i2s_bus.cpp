#include "i2s_bus.h"
#include "driver/i2s.h"
#include "waveform.h"
#include "flags.h"
#include "task_handles.h"


QueueHandle_t q_evt;
TaskHandle_t h_sampleTask = NULL;

void initI2S(void){
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = SR,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 1024,
        .use_apll = 0,
        .tx_desc_auto_clear = true,
        .fixed_mclk = -1};

    i2s_pin_config_t pin_config = {
        .bck_io_num = PIN_I2S_BCLK,
        .ws_io_num = PIN_I2S_LRC,
        .data_out_num = PIN_I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE};

    i2s_driver_install(I2S_NUM_0, &i2s_config, 4, &q_evt);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_zero_dma_buffer(I2S_NUM_0);

    xTaskCreate(Tsample, "i2s sample task", 4096, NULL, 1, &h_sampleTask);
    // xTaskCreate(ToutputController, "i2s output controller", 2048, NULL, 1, &h_outputTask);
}

void Tsample(void *param){
    int availableBytes = 0;
    int buffer_position = 0;
    frame_t *i2s_buf = (frame_t *)malloc(sizeof(frame_t) * NUM_FRAMES_TO_SEND);
    if(i2s_buf == NULL){
        yield();
    }

    while (true)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // wait for ON notification
        i2s_event_t evt;
        if (xQueueReceive(q_evt, &evt, portMAX_DELAY) == pdPASS) 
        {
            if (evt.type == I2S_EVENT_TX_DONE)
            {
                size_t bytesWritten = 0;
                do
                {
                    if(!f_i2s_is_on){ // exit condition
                        break;
                    }
                    if (availableBytes == 0)
                    {
                        getFrames(i2s_buf, NUM_FRAMES_TO_SEND);
                        availableBytes = NUM_FRAMES_TO_SEND * sizeof(uint32_t);
                        buffer_position = 0;
                    }
                    if (availableBytes > 0)
                    {
                        i2s_write(I2S_NUM_0, buffer_position + (uint8_t *)i2s_buf,
                                availableBytes, &bytesWritten, portMAX_DELAY);
                        availableBytes -= bytesWritten;
                        buffer_position += bytesWritten;
                    }
                } while (bytesWritten > 0);
            }
        }
    }
}
