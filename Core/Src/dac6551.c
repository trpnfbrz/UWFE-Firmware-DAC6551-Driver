#include "dac6551.h"

extern SPI_HandleTypeDef hspi2;

static inline void cs_low( dac6551_t * d )
{
	HAL_GPIO_WritePin( d->cs_port, d->cs_pin, GPIO_PIN_RESET );
}

static inline void cs_high( dac6551_t * d )
{
	HAL_GPIO_WritePin( d->cs_port, d->cs_pin, GPIO_PIN_SET );
}

void dac6551_init( dac6551_t * d )
{
	// Init CS to high
	cs_high (d);
	// First write: 0 V
	dac6551_write_code(d, 0);
}

HAL_StatusTypeDef dac6551_write_code( dac6551_t * d, uint16_t D_in )
{
    // clamp to 12-bit
    D_in &= 0x0FFF;

    uint32_t control_bits = ( 0x0 << 16 );
    uint32_t input_value = control_bits | ( D_in & 0x0FFF );

    cs_low( d ); // bring cs low to start write sequence
    HAL_StatusTypeDef status = HAL_SPI_Transmit ( &hspi2, ( uint8_t * ) & input_value, 3, HAL_MAX_DELAY );
    cs_high( d ); // bring cs high after write sequence

    return status;
}

HAL_StatusTypeDef dac6551_set_mv( dac6551_t * d, uint32_t mv )
{
    uint16_t code = ( ( uint64_t )mv * 4095UL ) / d->vref_mv;  // 4095 for 12-bit (0-4095)

    // clamp to 12 bit
    if( code > 4095 ) code = 4095;

    HAL_StatusTypeDef status = dac6551_write_code( d, code );

    return status;
}
