#ifndef FLASH_H
#define FLASH_H

#include "stm32f0xx_hal.h"  // 根据实际芯片型号修改头文件

class Flash
{
private:
    FLASH_EraseInitTypeDef m_handle;  // 擦除配置结构体
    uint32_t m_PageError;                  // 擦除错误页地址
    uint32_t m_totalSize;                  // Flash总容量(字节)
    uint32_t m_startAddr;                  // Flash起始地址
    uint32_t m_endAddr;                    // Flash结束地址
    bool isAddressValid(uint32_t addr, uint32_t length);
    uint32_t m_u32PageAddress;
public:
    Flash();
    Flash(FLASH_EraseInitTypeDef eraseHandle, uint32_t m_PageError);
    ~Flash();
    bool write(uint32_t dataAddr, const uint16_t* pData, int length);
    bool read(uint32_t dataAddr, uint16_t* pData, int length);
    uint16_t ReadHalfWord(uint32_t dataAddr);
    HAL_StatusTypeDef Program(uint32_t u32TypeProgram, uint32_t u32DeviceAddr, uint64_t Data);
    HAL_StatusTypeDef Erase();
    HAL_StatusTypeDef Lock();
    HAL_StatusTypeDef Unlock();
    void PageErase(uint32_t PageAddress);
	bool IsEnablePageError();
};

#endif // FLASH_H
