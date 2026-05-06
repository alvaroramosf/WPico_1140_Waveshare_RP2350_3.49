/* hw_config.c
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0(the License); you may not use 
this file except in compliance with the License.You may obtain a copy of the 
License at

   http : //www.apache.org/licenses/LICENSE-2.0 
Unless required by applicable law or agreed to in writing, software distributed 
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR 
CONDITIONS OF ANY KIND, either express or implied.See the License for the 
specific language governing permissions and limitations under the License.
*/

#include <string.h>
#include "my_debug.h"
#include "hw_config.h"
#include "ff.h"
#include "diskio.h"

void spi_dma_isr();

// Pinout verificado para Waveshare RP2350-Touch-LCD-3.49:
//   SD_SCLK → GPIO26  (SPI1 SCK)
//   SD_MOSI → GPIO27  (SPI1 TX)
//   SD_MISO → GPIO28  (SPI1 RX)
//   SD_CS   → GPIO31
static spi_t spis[] = {
{
    .hw_inst   = spi1,
    .miso_gpio = 28,
    .mosi_gpio = 27,
    .sck_gpio  = 26,
    .baud_rate = 4 * 1000 * 1000,
    .dma_isr   = spi_dma_isr
}
};

static sd_card_t sd_cards[] = {
{
    .pcName   = "0:",
    .spi      = &spis[0],
    .ss_gpio  = 31,
    .m_Status = STA_NOINIT
}
};

void spi_dma_isr() { spi_irq_handler(&spis[0]); }

size_t sd_get_num() { return count_of(sd_cards); }
sd_card_t *sd_get_by_num(size_t num) {
    if (num <= sd_get_num()) return &sd_cards[num];
    else return NULL;
}
size_t spi_get_num() { return count_of(spis); }
spi_t *spi_get_by_num(size_t num) {
    if (num <= sd_get_num()) return &spis[num];
    else return NULL;
}

/* [] END OF FILE */
