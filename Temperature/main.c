#include <LiquidCrystal.h>
#define TEMP_SENSOR A0
#define RED_LED 10
#define BLUE_LED 9
#define GREEN_LED 6
#define BUTTON 7

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int unit = '0';
float celsius = 0;
bool isFirstTime = true;
const long interval = 10000;
float totalCelsius = 0;
unsigned long initialTime = 0;
unsigned int addTimes = 0;

void setup()
{

    lcd.begin(16, 2);
    Serial.begin(9600);
    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BUTTON, INPUT);

    Serial.println("1.Display the temperature in fahrenheit");
    Serial.println("2.Display the temperature in celsius");
    Serial.println("3.Display the temperature in kelvin");
    Serial.println("4.Display the average temperature");
}

//function to control RGB
void RGBControl()
{
    if (celsius <= 0)
    {
        analogWrite(RED_LED, 0);
        analogWrite(GREEN_LED, 0);
        analogWrite(BLUE_LED, 255);
    }
    else if (celsius > 0 && celsius < 20)
    {
        float brightness = 255 - (255 / 20) * celsius;
        analogWrite(RED_LED, 0);
        analogWrite(GREEN_LED, 0);
        analogWrite(BLUE_LED, brightness);
    }
    else if (celsius >= 20 && celsius <= 30)
    {
        analogWrite(RED_LED, 0);
        analogWrite(GREEN_LED, 255);
        analogWrite(BLUE_LED, 0);
    }
    else if (celsius > 30 && celsius < 40)
    {
        float brightness = (255 / 10) * celsius - 30 * 25.5;
        analogWrite(RED_LED, brightness);
        analogWrite(GREEN_LED, 0);
        analogWrite(BLUE_LED, 0);
    }
    else if (celsius >= 40)
    {
        analogWrite(RED_LED, 255);
        analogWrite(GREEN_LED, 0);
        analogWrite(BLUE_LED, 0);
    }
}

//Function to calculate celsius
void calculateCelsius()
{
    int rawTemp = analogRead(TEMP_SENSOR);
    float rawVoltage = rawTemp * 5;
    rawVoltage = rawVoltage / 1024;
    celsius = rawVoltage - 0.5;
    celsius = celsius * 100;
}

//Function to use button change the units
void buttonControl()
{
    int buttonState = digitalRead(BUTTON);
    if (buttonState == HIGH)
    {
        if (unit == '4')
        {
            unit = '0';
            delay(100);
        }
        else
        {
            unit++;
            delay(100);
        }
    }

}

void loop()
{
    buttonControl();

    calculateCelsius();
    float fahrenheit = celsius * 1.8 + 32;
    float kelvin = celsius + 273.15;

    // Initial display
    if (unit == '0')
    {
        int rawTemp = analogRead(TEMP_SENSOR);
        calculateCelsius();
        float fahrenheit = celsius * 1.8 + 32;
        float kelvin = celsius + 273.15;
        lcd.setCursor(0, 0);
        lcd.print(celsius);
        lcd.print("C ");
        lcd.print(fahrenheit);
        lcd.print("F");
        lcd.setCursor(0, 1);
        lcd.print(kelvin);
        lcd.print("K");
        RGBControl();
        isFirstTime = true;
        totalCelsius = 0;
        addTimes = 0;

    }

    //get user input and print temperature in serial monitor
    int userInput = (int)Serial.read();
    //!= -1 because the userinput initial to be -1
    if (userInput != -1 && userInput == '1' || userInput == '2' || userInput == '3' || userInput == '4')
    {
        unit = userInput;
        if (unit == '1')
        {
            Serial.print("Current Fahrenheit: ");
            Serial.println(fahrenheit);
        }
        if (unit == '2')
        {
            Serial.print("Current Celsius: ");
            Serial.println(celsius);
        }
        if (unit == '3')
        {
            Serial.print("Current Kelvin: ");
            Serial.println(kelvin);
        }
        if (unit == '4')
        {
            Serial.println("Calculating....");
        }
    }

    //Display the temperature in fahrenheit
    if (unit == '1')
    {
        calculateCelsius();
        float fahrenheit = celsius * 1.8 + 32;
        lcd.clear();
        lcd.print(fahrenheit);
        lcd.print("F");
        RGBControl();
        isFirstTime = true;
        totalCelsius = 0;
        addTimes = 0;
        delay(100);
    }

    //Display the temperature in celsius
    else if (unit == '2')
    {
        calculateCelsius();
        lcd.clear();
        lcd.print(celsius);
        lcd.print("C");
        RGBControl();
        isFirstTime = true;
        totalCelsius = 0;
        addTimes = 0;
        delay(100);
    }

    //Display the temperature in kelvin
    else if (unit == '3')
    {
        calculateCelsius();
        float kelvin = celsius + 273.15;
        lcd.clear();
        lcd.print(kelvin);
        lcd.print("K");
        RGBControl();
        isFirstTime = true;
        totalCelsius = 0;
        addTimes = 0;
        delay(100);
    }

    //Display the average temperature (in all units)
    if (unit == '4')
    {
        if (isFirstTime)
        {
            initialTime = millis();
            isFirstTime = false;

        }

        calculateCelsius();
        totalCelsius += celsius;
        addTimes++;

        //calculate average temperature
        if (millis() - initialTime >= interval)
        {
            float averageCelsius = totalCelsius / addTimes;
            float averageKelvin = averageCelsius + 273.15;
            float averageFahrenheit = averageCelsius * 1.8 + 32;

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(averageCelsius);
            lcd.print("C ");
            lcd.print(averageKelvin);
            lcd.print("F");
            lcd.setCursor(0, 1);
            lcd.print(averageFahrenheit);
            lcd.print("K");
            isFirstTime = true;
            totalCelsius = 0;
            addTimes = 0;
            Serial.print("Average Celsius: ");
            Serial.println(averageCelsius);
            Serial.print("Average Fahrenheit: ");
            Serial.println(averageFahrenheit);
            Serial.print("Average Kelvin: ");
            Serial.println(averageKelvin);
        }

    }
}
