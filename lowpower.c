#include "lowpower.h"
#include "main.h"

void LowPower_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  HAL_PWR_ConfigSupply(PWR_LDO_SUPPLY);
  HAL_PWR_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
}
</doubaocanvas>
