#include "stm32f10x.h" 

//#define Input_floating  //uncomment if Input floating PA6 MISO Full duplex/master

void Spi1_Ini (void);
void Spi1_Tx_data (uint8_t data_tx);
void led_ini (void);

int main (void)
	{
		Spi1_Ini ();
		led_ini ();
		while(1)
			{
				Spi1_Tx_data (0x9A);
			}
	}

void Spi1_Tx_data (uint8_t data_tx)
	{
		while ((SPI1->SR&SPI_SR_BSY));
		SPI1->DR=data_tx;
	}

void led_ini (void)
	{
		RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;
		GPIOC->CRH&=~GPIO_CRH_CNF13;
		GPIOC->CRH|=GPIO_CRH_MODE13_0;
	}
	
void Spi1_Ini (void)
	{
		RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;			// clk apb2 bus
		RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;			// clk alternate function
		
		//PA5 SCK master Alternate function push-pull
		GPIOA->CRL &= ~ GPIO_CRL_CNF5;  //RESET CNF 0 BIT
		GPIOA->CRL|=GPIO_CRL_CNF5_1;			//SET CNF 1 BIT
		GPIOA->CRL|=GPIO_CRL_MODE5_1;				//2MHz
		
		//PA7 MOSI Full duplex / master Alternate function push-pull
		GPIOA->CRL&=~GPIO_CRL_CNF7;			//RESET CNF 0 BIT
		GPIOA->CRL|=GPIO_CRL_CNF7_1;			//SET CNF 1 BIT
		GPIOA->CRL|=GPIO_CRL_MODE7;
		
		//PA6 MISO Full duplex / master Input pull-up 
		#ifndef Input_floating
		GPIOA->CRL&=~GPIO_CRL_CNF6;
		GPIOA->CRL|=GPIO_CRL_CNF1;
		GPIOA->BSRR   =  GPIO_BSRR_BS6;
		#endif
		
		RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;			// clk module SPI

		SPI1->CR1 |= SPI_CR1_BR;
		SPI1->CR1 |= SPI_CR1_SSM;
		SPI1->CR1 |= SPI_CR1_SSI;
		SPI1->CR1 |= SPI_CR1_MSTR;
		SPI1->CR1 |= SPI_CR1_CPOL;  //log 1 then waiting
		SPI1->CR1 |= SPI_CR1_SPE;
	}
