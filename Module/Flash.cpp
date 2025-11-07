#include "Flash.h"

Flash::Flash()
{
}


Flash::Flash(FLASH_EraseInitTypeDef handle,uint32_t PageError)
  : m_handle(handle)
	, m_PageError(PageError)
{
}


Flash::~Flash()
{
    Lock();
}


HAL_StatusTypeDef Flash::Program(uint32_t TypeProgram, uint32_t u32DataAddr, uint64_t u64Data)
{
	return HAL_FLASH_Program(TypeProgram,u32DataAddr,u64Data);
}


HAL_StatusTypeDef Flash::Erase()
{
	HAL_FLASH_Unlock();
	HAL_StatusTypeDef success=HAL_FLASHEx_Erase(&m_handle,&m_PageError);
	HAL_FLASH_Lock();
	return success;
}


HAL_StatusTypeDef Flash::Lock()
{
	return HAL_FLASH_Lock();
}
HAL_StatusTypeDef Flash::Unlock()
{
	return HAL_FLASH_Unlock();
}


bool Flash::write(uint32_t dataAddr, const uint16_t* pData, int length)
{
    if (pData == nullptr || length <= 0)
        return false;
    if (Unlock() != HAL_OK)
        return false;
    bool writeSuccess = true;
    uint64_t tempBuff=0;
    for (int i = 0; i < length; ++i)
    {
        tempBuff=*(pData+i);
        HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,dataAddr + i * 2, tempBuff);
        if (status != HAL_OK)
        {
            writeSuccess = false;
            break;
        }
    }
    Lock();
    return writeSuccess;
}


uint16_t Flash::ReadHalfWord(uint32_t u32DataAddr)
{
	return *(__IO uint16_t*)u32DataAddr;
}


bool Flash::read(uint32_t dataAddr, uint16_t* pData, int length)
{
    if (pData == nullptr || length <= 0)
        return false;
    for (int i = 0; i < length; ++i)
    {
        pData[i] = ReadHalfWord(dataAddr + i * 2);
    }
    return true;
}