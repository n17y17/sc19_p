/**************************************************
 * ADCによる入力に関するコードです
 * このファイルは，adc.hppに名前だけ書かれている関数の中身です
 * 
**************************************************/

//! @file adc.cpp
//! @brief ADCによる入力

#include "adc.hpp"

namespace sc
{

/***** class ADC *****/

ADC::ADC(Pin adc_pin) try :
    _adc_pin(adc_pin), _channel(ADC::get_channel(adc_pin))
{
    #ifndef NODEBUG
    std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif 
    try
    {   
        if (!(Pin::Status.at(_adc_pin.gpio()) == PinStatus::NoUse))
        {
    throw std::invalid_argument(f_err(__FILE__, __LINE__, "Pin %hhu is already in use", _adc_pin.gpio()));  // このピンは既に使用されています
        } else if (ADC::IsUse[_channel]) {
    throw std::logic_error(f_err(__FILE__, __LINE__, "ADC %hhu cannot be reinitialized", _channel));  // ADCを再度初期化することはできません
        }

        Pin::Status.at(_adc_pin.gpio()) = PinStatus::Adc;

        if (ADC::IsUse[0]==false && ADC::IsUse[1]==false && ADC::IsUse[2]==false && ADC::IsUse[3]==false && ADC::IsUse[4]==false)
        {
            ::adc_init();
        }

        ADC::IsUse[_channel] = true;

        ::adc_gpio_init(_adc_pin.gpio());
    }
    catch(const std::exception& e)
    {
        save = false;
        print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n%s\n\n********************\n", __FILE__, __LINE__, e.what());
    }
}
catch (const std::exception& e)
{
    print(f_err(__FILE__, __LINE__, e, "An initialization error occurred"));
}

uint8_t ADC::get_channel(Pin pin)
{
    #ifndef NODEBUG
    std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return pin.gpio() - 26;
}

uint16_t ADC::read() const
{
    #ifndef NODEBUG
    std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (save == false)
        throw std::logic_error(f_err(__FILE__, __LINE__, "Cannot execute because initialization failed"));
    ::adc_select_input(_channel);
    return ::adc_read();
}


/***** class PicoTemp *****/

PicoTemp::PicoTemp() try
{
    #ifndef NODEBUG
    std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    try
    {
        if (ADC::IsUse[0]==false && ADC::IsUse[1]==false && ADC::IsUse[2]==false && ADC::IsUse[3]==false && ADC::IsUse[4]==false)
        {
            ::adc_init();
        }

        ADC::IsUse[4] = true;

        ::adc_set_temp_sensor_enabled(true);  // 温度センサを有効にする
    }
    catch(const std::exception& e)
    {
        save = false;
        print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n%s\n\n********************\n", __FILE__, __LINE__, e.what());
    }
}
catch (const std::exception& e)
{
    print(f_err(__FILE__, __LINE__, e, "An initialization error occurred"));
}

dimension::degC PicoTemp::read()
{
    #ifndef NODEBUG
    std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (save == false)
        throw std::logic_error(f_err(__FILE__, __LINE__, "Cannot execute because initialization failed"));
    ::adc_select_input(4);
    double read_temp = 27 - ((::adc_read() * 3.3 / (1<<12)) - 0.706)/0.001721;
    print("pico_temp_data:%f\n", read_temp);
    return dimension::degC(read_temp);
}


/***** class VsysVoltage *****/

VsysVoltage::VsysVoltage() try
{
    #ifndef NODEBUG
    std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    try
    {
        if (ADC::IsUse[0]==false && ADC::IsUse[1]==false && ADC::IsUse[2]==false && ADC::IsUse[3]==false && ADC::IsUse[4]==false)
        {
            ::adc_init();
        }

        ADC::IsUse[3] = true;

        ::adc_gpio_init(29);
    }
    catch(const std::exception& e)
    {
        save = false;
        print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n%s\n\n********************\n", __FILE__, __LINE__, e.what());
    }
}
catch (const std::exception& e)
{
    print(f_err(__FILE__, __LINE__, e, "An initialization error occurred"));
}

dimension::V VsysVoltage::read()
{
    #ifndef NODEBUG
    std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (save == false)
        throw std::logic_error(f_err(__FILE__, __LINE__, "Cannot execute because initialization failed"));
    ::adc_select_input(3);
    double voltage = 3 * ::adc_read() * 3.3 / (1 << 12);
    print("vsys_voltage_data:%f\n", voltage);
    return dimension::V(voltage);
}


}