#include<stdio.h>
#include<stdlib.h>
#include <WinSock2.h>
#include <iostream>
#pragma comment (lib,"Ws2_32.lib")
#pragma warning(disable : 4996) 

bool emptyMac(char* MAC)
{
    for (int i = 0; i < 6; i++)
        if (MAC[i] != 0)
            return false;
    return true;
}

int main()
{
    //Файл с входными данными
    FILE* ethers = NULL;

    // имя файла
    char filename[100];

    // Размер файла
    int fileSize = 0;

    // числе фремов
    int frameNumber = 1;

    // число кадров формата ARP
    int ARP = 0;
    
    // число кадров формата IPv4
    int IPv4 = 0;

    // число кадров формата DIX
    int DIX = 0;

    // число кадров формата SNAP
    int SNAP = 0;

    // число кадров формата RAW
    int RAW = 0;

    // число кадров формата LLC
    int LLC = 0;

    // Указатель на массив данных из файла
    char* frames;

    // ввод имени файла и его октрытие
    while (true)
    {
        // ввод имени файла
        std::cout << "Enter the file name: ";
        std::cin >> filename ;
        // открытие файла
        ethers = fopen(filename, "rb");
        // если нашли такой файл, то идём дальше, нет ввод имени заного
        if (ethers != NULL)
            break;
        else 
            std::cout << std::endl << "This does not exist! Try again." << std::endl;
    }

    // определить размер данных
    // переместить указатель файла в конец
    fseek(ethers, 0, SEEK_END);
    // считать его позицию
    fileSize = ftell(ethers);
    // вернуть указатель в начало файла
    fseek(ethers, 0, SEEK_SET);

    // считать данные из файла
    // выделяем память под массив данных
    frames = new char[fileSize];
    // читаем данные из файла
    fread(frames, fileSize, 1, ethers);
    // закрысваем файл
    fclose(ethers);


    // вывод размера даннных
    std::cout << std::endl << "Size of file: " << fileSize << " bytes " << std::endl;
    std::cout << std::endl << "--------------------------------------------------------" << std::endl;

    //текущее положение указателя в нашем массиве данных
    char* currentIndex = frames; 
    while (currentIndex < frames + fileSize) //пока не конец данных
    {
        // выводим номер фрейма
        std::cout << "Frame number: " << frameNumber << std::endl;
        std::cout << "MAC dest: "; 
        while (emptyMac(currentIndex))
            currentIndex += 6;
        // mac адресс адресата
        // вывод мас адреса
        for (int i = 0; i < 6; i++)
        {
            // берём каждый байт и переводим его в шестнадциричную систему и выводим два его символа
            printf("%02X", (unsigned char)currentIndex[i]);
            if (i < 5) std::cout << ":";
        }
        // мас адресс отправителя
        std::cout << std::endl << "MAC source: "; // мас адресс отправителя
        // вывод мас адреса
        for (int i = 0; i < 6; i++)
        {
            // берём каждый байт  со смещением на 6 байт и переводим его в шестнадциричную систему и выводим два его символа
            printf("%02X", (unsigned char)(currentIndex + 6)[i]); // со смещением на 6 байт
            if (i < 5) std::cout << ":";
        }

        //значения каждого байта кадра Ethernet нужно зеркально отобразить, 
        //чтобы получить правильное представление о значении разрядов этого 
        //байта в соответствии с документами IEEE

        unsigned short Length = ntohs(*(unsigned short*)(currentIndex + 12)); //обратный порядок байт
        // считывает два байта unsigned short со смещением в 12 байт (два мас адреса) 
        //и отзеркаливает их биты функцией network to host short (ntohs)
         
        if (Length == 0x800) // 0x800 - значение типа поля протокола для IPv4
        { 
            // вывод айпи адреса адресета и отправителя
            std::cout << std::endl << "Frame type: IPv4" << std::endl;
            std::cout << "IP source: ";
            // вывод IP-адреса адресата
            for (int i = 0; i < 4; i++)
            {
                // берём каждый байт со смещением на 26 байт 
                //26 байт = 12 байт-mac адрес + 2 байта - тип кадра +  12 байт - информация IP-дейтограмме
                // переводим его в шестнадциричную систему и выводим два его символа
                printf("%d", (unsigned char)(currentIndex + 26)[i]);
                if (i < 3) std::cout << ".";
            }
            std::cout << std::endl << "IP dest: ";
            // вывод IP-адреса отправителя
            for (int i = 0; i < 4; i++)
            {
                // берём каждый байт со смещением на 30 байт 
                //30 байт = 12 байт-mac адрес + 2 байта - тип кадра + 12 байт - информация IP-дейтограмме + 4 байта IP-адрес адресата
                // переводим его в шестнадциричную систему и выводим два его символа
                printf("%d", (unsigned char)(currentIndex + 30)[i]);
                if (i < 3) std::cout << ".";
            }
            std::cout << std::endl;
            // получаем значение полной длины дейтаграммы + 
            // сдвиг указателя в массиве до следующего фрейма (длина дейтаграммы + 12 байтов адресов и 2 байта типа)
            Length = ntohs(*(unsigned short*)(currentIndex + 16)) + 14;
            std::cout << std::endl << "--------------------------------------------------------" << std::endl;
            // применяем сдвиг
            currentIndex += Length;
            // Увеличиваем счётчик для типа IPv4 на 1
            IPv4++;
            // Увеличиваем счётчик фреймов на 1
            frameNumber++;
        }
        else
        {
            if (Length == 0x0806) // 0x0806 - значение типа поля протокола для IPv4arp 2054 
            {
                // Выводим имнформация о типе фрейма
                std::cout << std::endl << "Frame type: ARP" << std::endl;
                // сдвиг указателя в массиве до следующего фрейма (длина дейтаграммы + 12 байтов адресов и 2 байта типа)
                currentIndex += 28 + 14;
                // Увеличиваем счётчик для типа ARP на 1
                ARP++;
                std::cout << std::endl << "--------------------------------------------------------" << std::endl;
                // Увеличиваем счётчик фреймов на 1
                frameNumber++;
            }
            else
            {
                // если записанное значение в поле типа протокола превышает 0x05FE (максимально возможная длина
                //кадра), то это кадр Ethernet II
                if (Length > 0x05FE)
                {
                    std::cout << std::endl << "Frame type: Ethernet DIX (Ethernet II)" << std::endl;
                    // Увеличиваем счётчик для типа DIX на 
                    DIX++;
                }
                //Если значение поля типа не превышает величину, следует продолжить проверку на другие типы фрейма
                else
                {
                    //  значение первых двух байтов из поля данных для конкретизации тип
                    unsigned short F = ntohs(*(unsigned  short*)(currentIndex + 14));
                    // если значение первых двух байтов из поля данных равно 0xFFFF - это фрейм типа Ethernet 802.3
                    if (F == 0xFFFF)//raw 65535 
                    {
                        // Выводим имнформация о типе фрейма
                        std::cout << std::endl << "Frame type: Raw 802.3 (Frame Novell 802.3)" << std::endl;
                        // Увеличиваем счётчик для типа RAW на 1
                        RAW++;
                    }
                    else
                        // если значение первых двух байтов из поля данных равно 0xАААА - это фрейм типа Ethernet SNAP (snap 43690) 
                        if (F == 0xAAAA) //snap 43690 
                        {
                            // Выводим имнформация о типе фрейма
                            std::cout << std::endl << "Frame type: Ethernet SNAP" << std::endl;
                            // Увеличиваем счётчик для типа SNAP на 1
                            SNAP++;
                        }
                        else //llc 
                        {
                            // Выводим имнформация о типе фрейма
                            std::cout << std::endl << "Frame type: 802.3/LLC (Frame type 802.3/802.2 or Frame type Novell 802.2)" << std::endl;
                            // Увеличиваем счётчик для типа LLC на 1
                            LLC++;
                        }
                }
                std::cout << std::endl << "--------------------------------------------------------" << std::endl;
                // сдвиг указателя в массиве до следующего фрейма (длина дейтаграммы + 12 байтов адресов и 2 байта типа)
                currentIndex += Length + 14;
                // Увеличиваем счётчик фреймов на 1
                frameNumber++;
            }
        }
    }

    // Вывод данных на консоль
    std::cout << "Total number of frames: " << frameNumber - 1 << std::endl;
    std::cout << "IPv4: " << IPv4 << std::endl;
    std::cout << "ARP:  " << ARP << std::endl;
    std::cout << "DIX:  " << DIX << std::endl;
    std::cout << "RAW:  " << RAW << std::endl;
    std::cout << "SNAP: " << SNAP << std::endl;
    std::cout << "LLC:  " << LLC << std::endl;

    std::cout << "Done!";
    system("pause");
    
    return 0;  // Added return statement
}