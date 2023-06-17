
/* Includes ------------------------------------------------------------------*/
#include "GPIO_Driver.h"


#define GPIO_NUMBER           (16u)

/**
  * @brief  Initialize the GPIOx peripheral according to the specified parameters in the GPIO_Config.
  * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32G0xx family
  * @param  GPIO_Config pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void GPIO_Config(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
  uint32_t position = 0x00u;
  uint32_t iocurrent;
  uint32_t temp;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Init->Pin));
  assert_param(IS_GPIO_MODE(GPIO_Init->Mode));

  /* Configure the port pins */
  while (((GPIO_Init->Pin) >> position) != 0x00u)
  {
    /* Get current io position */
    iocurrent = (GPIO_Init->Pin) & (1uL << position);

    if (iocurrent != 0x00u)
    {
      /*--------------------- GPIO Mode Configuration ------------------------*/
      /* In case of Output or Alternate function mode selection */
      if (((GPIO_Init->Mode & GPIO_MODE) == MODE_OUTPUT) || ((GPIO_Init->Mode & GPIO_MODE) == MODE_AF))
      {
        /* Check the Speed parameter */
        assert_param(IS_GPIO_SPEED(GPIO_Init->Speed));

        /* Configure the IO Speed */
        temp = GPIOx->OSPEEDR;
        temp &= ~(GPIO_OSPEEDR_OSPEED0 << (position * 2u));
        temp |= (GPIO_Init->Speed << (position * 2u));
        GPIOx->OSPEEDR = temp;

        /* Configure the IO Output Type */
        temp = GPIOx->OTYPER;
        temp &= ~(GPIO_OTYPER_OT0 << position) ;
        temp |= (((GPIO_Init->Mode & OUTPUT_TYPE) >> OUTPUT_TYPE_Pos) << position);
        GPIOx->OTYPER = temp;
      }

      if ((GPIO_Init->Mode & GPIO_MODE) != MODE_ANALOG)
      {
        /* Check the Pull parameter */
        assert_param(IS_GPIO_PULL(GPIO_Init->Pull));

        /* Activate the Pull-up or Pull down resistor for the current IO */
        temp = GPIOx->PUPDR;
        temp &= ~(GPIO_PUPDR_PUPD0 << (position * 2u));
        temp |= ((GPIO_Init->Pull) << (position * 2u));
        GPIOx->PUPDR = temp;
      }

      /* In case of Alternate function mode selection */
      if ((GPIO_Init->Mode & GPIO_MODE) == MODE_AF)
      {
        /* Check the Alternate function parameters */
        assert_param(IS_GPIO_AF_INSTANCE(GPIOx));
        assert_param(IS_GPIO_AF(GPIO_Init->Alternate));

        /* Configure Alternate function mapped with the current IO */
        temp = GPIOx->AFR[position >> 3u];
        temp &= ~(0xFu << ((position & 0x07u) * 4u));
        temp |= ((GPIO_Init->Alternate) << ((position & 0x07u) * 4u));
        GPIOx->AFR[position >> 3u] = temp;
      }

      /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
      temp = GPIOx->MODER;
      temp &= ~(GPIO_MODER_MODE0 << (position * 2u));
      temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2u));
      GPIOx->MODER = temp;

      /*--------------------- EXTI Mode Configuration ------------------------*/
      /* Configure the External Interrupt or event for the current IO */
      if ((GPIO_Init->Mode & EXTI_MODE) != 0x00u)
      {
        temp = EXTI->EXTICR[position >> 2u];
        temp &= ~(0x0FuL << (8u * (position & 0x03u)));
        temp |= (GPIO_GET_INDEX(GPIOx) << (8u * (position & 0x03u)));
        EXTI->EXTICR[position >> 2u] = temp;

        /* Clear Rising Falling edge configuration */
        temp = EXTI->RTSR1;
        temp &= ~(iocurrent);
        if ((GPIO_Init->Mode & TRIGGER_RISING) != 0x00u)
        {
          temp |= iocurrent;
        }
        EXTI->RTSR1 = temp;

        temp = EXTI->FTSR1;
        temp &= ~(iocurrent);
        if ((GPIO_Init->Mode & TRIGGER_FALLING) != 0x00u)
        {
          temp |= iocurrent;
        }
        EXTI->FTSR1 = temp;

        /* Clear EXTI line configuration */
        temp = EXTI->EMR1;
        temp &= ~(iocurrent);
        if ((GPIO_Init->Mode & EXTI_EVT) != 0x00u)
        {
          temp |= iocurrent;
        }
        EXTI->EMR1 = temp;

        temp = EXTI->IMR1;
        temp &= ~(iocurrent);
        if ((GPIO_Init->Mode & EXTI_IT) != 0x00u)
        {
          temp |= iocurrent;
        }
        EXTI->IMR1 = temp;
      }
    }

    position++;
  }
}

/**
  * @brief  De-initialize the GPIOx peripheral registers to their default reset values.
  * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32G0xx family
  * @param  GPIO_Pin specifies the port bit to be written.
  *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval None
  */
void GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
  uint32_t position = 0x00u;
  uint32_t iocurrent;
  uint32_t tmp;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  /* Configure the port pins */
  while ((GPIO_Pin >> position) != 0x00u)
  {
    /* Get current io position */
    iocurrent = (GPIO_Pin) & (1uL << position);

    if (iocurrent != 0x00u)
    {
      /*------------------------- EXTI Mode Configuration --------------------*/
      /* Clear the External Interrupt or Event for the current IO */

      tmp = EXTI->EXTICR[position >> 2u];
      tmp &= (0x0FuL << (8u * (position & 0x03u)));
      if (tmp == (GPIO_GET_INDEX(GPIOx) << (8u * (position & 0x03u))))
      {
        /* Clear EXTI line configuration */
        EXTI->IMR1 &= ~(iocurrent);
        EXTI->EMR1 &= ~(iocurrent);

        /* Clear Rising Falling edge configuration */
        EXTI->FTSR1 &= ~(iocurrent);
        EXTI->RTSR1 &= ~(iocurrent);

        tmp = 0x0FuL << (8u * (position & 0x03u));
        EXTI->EXTICR[position >> 2u] &= ~tmp;
      }

      /*------------------------- GPIO Mode Configuration --------------------*/
      /* Configure IO in Analog Mode */
      GPIOx->MODER |= (GPIO_MODER_MODE0 << (position * 2u));

      /* Configure the default Alternate Function in current IO */
      GPIOx->AFR[position >> 3u] &= ~(0xFu << ((position & 0x07u) * 4u)) ;

      /* Configure the default value for IO Speed */
      GPIOx->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED0 << (position * 2u));

      /* Configure the default value IO Output Type */
      GPIOx->OTYPER  &= ~(GPIO_OTYPER_OT0 << position) ;

      /* Deactivate the Pull-up and Pull-down resistor for the current IO */
      GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPD0 << (position * 2u));
    }

    position++;
  }
}



/**
  * @brief  Read the specified input port pin.
  * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32G0xx family
  * @param  GPIO_Pin specifies the port bit to read.
  *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval The input port pin value.
  */
GPIO_PinState GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  GPIO_PinState bitstatus;

  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  if ((GPIOx->IDR & GPIO_Pin) != 0x00u)
  {
    bitstatus = GPIO_PIN_SET;
  }
  else
  {
    bitstatus = GPIO_PIN_RESET;
  }
  return bitstatus;
}

/**
  * @brief  Set or clear the selected data port bit.
  *
  * @note   This function uses GPIOx_BSRR and GPIOx_BRR registers to allow atomic read/modify
  *         accesses. In this way, there is no risk of an IRQ occurring between
  *         the read and the modify access.
  *
  * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32G0xx family
  * @param  GPIO_Pin specifies the port bit to be written.
  *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @param  PinState specifies the value to be written to the selected bit.
  *         This parameter can be one of the GPIO_PinState enum values:
  *            @arg GPIO_PIN_RESET: to clear the port pin
  *            @arg GPIO_PIN_SET: to set the port pin
  * @retval None
  */

void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_PIN_ACTION(PinState));

  if (PinState != GPIO_PIN_RESET)
  {
    GPIOx->BSRR = (uint32_t)GPIO_Pin;
  }
  else
  {
    GPIOx->BRR = (uint32_t)GPIO_Pin;
  }
}

/**
  * @brief  Toggle the specified GPIO pin.
  * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32G0xx family
  * @param  GPIO_Pin specifies the pin to be toggled.
  *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval None
  */
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  uint32_t odr;

  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  /* get current Output Data Register value */
  odr = GPIOx->ODR;

  /* Set selected pins that were at low level, and reset ones that were high */
  GPIOx->BSRR = ((odr & GPIO_Pin) << GPIO_NUMBER) | (~odr & GPIO_Pin);
}

/**
* @brief  Lock GPIO Pins configuration registers.
  * @note   The locked registers are GPIOx_MODER, GPIOx_OTYPER, GPIOx_OSPEEDR,
  *         GPIOx_PUPDR, GPIOx_AFRL and GPIOx_AFRH.
  * @note   The configuration of the locked GPIO pins can no longer be modified
  *         until the next reset.
  * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32G0xx family
  * @param  GPIO_Pin specifies the port bits to be locked.
  *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval None
  */
StatusTypeDef GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  __IO uint32_t tmp = GPIO_LCKR_LCKK;

  /* Check the parameters */
  assert_param(IS_GPIO_LOCK_INSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  /* Apply lock key write sequence */
  tmp |= GPIO_Pin;
  /* Set LCKx bit(s): LCKK='1' + LCK[15-0] */
  GPIOx->LCKR = tmp;
  /* Reset LCKx bit(s): LCKK='0' + LCK[15-0] */
  GPIOx->LCKR = GPIO_Pin;
  /* Set LCKx bit(s): LCKK='1' + LCK[15-0] */
  GPIOx->LCKR = tmp;
  /* Read LCKK register. This read is mandatory to complete key lock sequence */
  tmp = GPIOx->LCKR;

  /* read again in order to confirm lock is active */
  if ((GPIOx->LCKR & GPIO_LCKR_LCKK) != 0x00u)
  {
    return OK;
  }
  else
  {
    return BIT_ERROR;
  }
}

