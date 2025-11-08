#include<stdio.h>
#include<stdlib.h>
#include <WinSock2.h>
#include <iostream>
#pragma comment (lib,"Ws2_32.lib")
#pragma warning(disable : 4996) 

bool emptyMac(char* MAC){
    for (int i = 0; i < 5; i++)
        if (MAC[i] != 0)
            return false;
    return true;
}

int main(){
    FILE* ethers = NULL;
    char filename[100];
    char* frames;

    int fileSize = 0; //размер файла
    int frameNumber = 1; //число фреймов
    int ARP = 0; //число кадров формата ARP
    int IPv4 = 0; //число кадров формата IPv4
    int DIX = 0; //число кадров формата DIX
    int SNAP = 0; //число кадров формата SNAP
    int RAW = 0; //число кадров формата RAW
    int LLC = 0; //число кадров формата LLC

    while (true){
        std::cout << "Enter the file name: ";
        std::cin >> filename ;
        ethers = fopen(filename, "rb");
        if (ethers != NULL)
            break;
        else 
            std::cout << std::endl << "This does not exist! Try again." << std::endl;
    }

    //определить размер данных
    //переместить указатель файла в конец
    fseek(ethers, 0, SEEK_END);
    //считать его позицию
    fileSize = ftell(ethers);
    //вернуть указатель в начало
    fseek(ethers, 0, SEEK_SET);

    //считать данные из файла
    //выделяем память под массив данных
    frames = new char[fileSize];
    //читаем данные из файла
    fread(frames, fileSize, 1, ethers);
    //закрысваем файл
    fclose(ethers);

    std::cout << std::endl << "Size of file: " << fileSize << " bytes " << std::endl;
    std::cout << std::endl << "--------------------------------------------------------" << std::endl;

    //текущее положение указателя в нашем массиве данных
    char* currentIndex = frames; 
    while (currentIndex < frames + fileSize){
        std::cout << "Frame number: " << frameNumber << std::endl;
        std::cout << "MAC dest: "; 
        while (emptyMac(currentIndex))
            currentIndex += 6;

        for (int i = 0; i < 6; i++){
            //берём каждый байт и переводим его в шестнадциричную систему и выводим два его символа
            printf("%02X", (unsigned char)currentIndex[i]);
            if (i < 5) std::cout << ":";
        }
        
        std::cout << std::endl << "MAC source: ";

        for (int i = 0; i < 6; i++){
            //берём каждый байт со смещением на 6 байт и переводим его в шестнадциричную систему и выводим два его символа
            printf("%02X", (unsigned char)(currentIndex + 6)[i]);
            if (i < 5) std::cout << ":";
        }

        unsigned short Length = ntohs(*(unsigned short*)(currentIndex + 12)); //обратный порядок байт
         
        if (Length == 0x800){ // 0x800 - значение типа поля протокола для IPv4
            //вывод айпи адреса адресета и отправителя
            std::cout << std::endl << "Frame type: IPv4" << std::endl;
            std::cout << "IP source: ";
            //вывод IP-адреса адресата
            for (int i = 0; i < 4; i++)
            {
                //берём каждый байт со смещением на 26 байт 
                //26 байт = 12 байт-mac адрес + 2 байта - тип кадра +  12 байт - информация IP-дейтограмме
                //переводим его в шестнадциричную систему и выводим два его символа
                printf("%d", (unsigned char)(currentIndex + 26)[i]);
                if (i < 3) std::cout << ".";
            }
            std::cout << std::endl << "IP dest: ";
            //вывод IP-адреса отправителя
            for (int i = 0; i < 4; i++)
            {
                /*берём каждый байт со смещением на 30 байт 
                30 байт = 12 байт-mac адрес + 2 байта - тип кадра + 12 байт - информация IP-дейтограмме + 4 байта IP-адрес адресата
                переводим его в шестнадциричную систему и выводим два его символа*/
                printf("%d", (unsigned char)(currentIndex + 30)[i]);
                if (i < 3) std::cout << ".";
            }
            std::cout << std::endl;
            /*получаем значение полной длины дейтаграммы + 
            сдвиг указателя в массиве до следующего фрейма (длина дейтаграммы + 12 байтов адресов и 2 байта типа)*/
            Length = ntohs(*(unsigned short*)(currentIndex + 16)) + 14;
            std::cout << std::endl << "--------------------------------------------------------" << std::endl;
            //применяем сдвиг
            currentIndex += Length;
            //увеличиваем счётчик для типа IPv4 на 1
            IPv4++;
            //увеличиваем счётчик фреймов на 1
            frameNumber++;
        }
        else
        {
            if (Length == 0x0806){ // 0x0806 - значение типа поля протокола для IPv4arp 2054 
                std::cout << std::endl << "Frame type: ARP" << std::endl;
                // сдвиг указателя в массиве до следующего фрейма (длина дейтаграммы + 12 байтов адресов и 2 байта типа)
                currentIndex += 28 + 14;
                ARP++;
                std::cout << std::endl << "--------------------------------------------------------" << std::endl;
                frameNumber++;
            }
            else{
                /*если записанное значение в поле типа протокола превышает 0x05FE (максимально возможная длина
                кадра), то это кадр Ethernet II*/
                if (Length > 0x05FE){
                    std::cout << std::endl << "Frame type: Ethernet DIX (Ethernet II)" << std::endl;
                    DIX++;
                }
                //если значение поля типа не превышает величину, следует продолжить проверку на другие типы фрейма
                else{
                    unsigned short F = ntohs(*(unsigned  short*)(currentIndex + 14));
                    //если значение первых двух байтов из поля данных равно 0xFFFF - это фрейм типа Ethernet 802.3
                    if (F == 0xFFFF){
                        std::cout << std::endl << "Frame type: Raw 802.3 (Frame Novell 802.3)" << std::endl;
                        RAW++;
                    }else{
                        //если значение первых двух байтов из поля данных равно 0xАААА - это фрейм типа Ethernet SNAP (snap 43690) 
                        if (F == 0xAAAA){
                            std::cout << std::endl << "Frame type: Ethernet SNAP" << std::endl;
                            SNAP++;
                        }
                        else{
                            std::cout << std::endl << "Frame type: 802.3/LLC (Frame type 802.3/802.2 or Frame type Novell 802.2)" << std::endl;
                            LLC++;
                        }
                    }
                }
                std::cout << std::endl << "--------------------------------------------------------" << std::endl;
                //сдвиг указателя в массиве до следующего фрейма (длина дейтаграммы + 12 байтов адресов и 2 байта типа)
                currentIndex += Length + 14;
                frameNumber++;
            }
        }
    }

    std::cout << "Total number of frames: " << frameNumber - 1 << std::endl;
    std::cout << "IPv4: " << IPv4 << std::endl;
    std::cout << "ARP:  " << ARP << std::endl;
    std::cout << "DIX:  " << DIX << std::endl;
    std::cout << "RAW:  " << RAW << std::endl;
    std::cout << "SNAP: " << SNAP << std::endl;
    std::cout << "LLC:  " << LLC << std::endl;

    std::cout << "Done!";
    system("pause");
    
    return 0;
}