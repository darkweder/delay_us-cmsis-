#include "stm32f10x.h"
#define SYSCLOCK 72000000U

//----------------------------------------------------------
__IO uint32_t tmpreg;
__IO uint32_t SysTick_CNT = 0;
//----------------------------------------------------------
__forceinline void delay(__IO uint32_t tck)
{
  while(tck)
  {
    tck--;
  }  
}
//----------------------------------------------------------
void delay_ms(uint32_t ms)
{
	SetSysClockTo72();
      SysTick_Init();
  MODIFY_REG(SysTick->VAL,SysTick_VAL_CURRENT_Msk,SYSCLOCK / 1000 - 1);
  SysTick_CNT = ms;
  while(SysTick_CNT) {}
		
}
//-----------------------------------------------------------
void RCC_DeInit(void)
{
  SET_BIT(RCC->CR, RCC_CR_HSION);
  while(READ_BIT(RCC->CR, RCC_CR_HSIRDY == RESET)) {}
  MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U);
  CLEAR_REG(RCC->CFGR);
  while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET) {}
  CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
  while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET) {}
  CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
  while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET) {}
  CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
  //Reset all CSR flags
  SET_BIT(RCC->CSR, RCC_CSR_RMVF);
}
//----------------------------------------------------------
void SetSysClockTo72(void)
{
  SET_BIT(RCC->CR, RCC_CR_HSEON);
  while(READ_BIT(RCC->CR, RCC_CR_HSERDY == RESET)) {}
  //Enable the Prefetch Buffer
  CLEAR_BIT(FLASH->ACR, FLASH_ACR_PRFTBE);
  SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE);
  MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_2);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL,
             RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9);
  SET_BIT(RCC->CR, RCC_CR_PLLON);
  while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) != (RCC_CR_PLLRDY)) {}
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
  while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
}
//----------------------------------------------------------
void SysTick_Init(void)
{
  MODIFY_REG(SysTick->LOAD,SysTick_LOAD_RELOAD_Msk,SYSCLOCK / 1000 - 1);
  CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);
  SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

			//----------------------------------------------------------


	void SysTick_Handler(void)
{
  if(SysTick_CNT > 0)  SysTick_CNT--;
}

