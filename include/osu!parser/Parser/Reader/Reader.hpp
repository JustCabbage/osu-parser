#pragma once
#include <fstream>
#include <iostream>
#include <any>

namespace OsuParser
{
    class Reader
    {
    public:

        Reader() {}
        
        Reader(const std::string& StreamPath)
        {
            this->SetStream(StreamPath);
        }

        ~Reader()
        {
            if(this->m_CurrentStream.is_open())
            {
                this->m_CurrentStream.close();
            }
        }

        bool SetStream(const std::string& StreamPath)
        {
            if(this->m_CurrentStream.is_open())
            {
                this->m_CurrentStream.close();
            }
            this->m_CurrentStream = std::ifstream(StreamPath, std::ios::binary);
            return this->m_CurrentStream.good();
        }

        std::uint64_t ReadUleb128()
        {
            std::uint8_t Byte = 0x0;
            std::uint32_t Result = 0;
            std::int32_t ShiftAmount = 0;
            do 
            {
                Byte = this->ReadType<std::uint8_t>();
                Result |= (Byte & 0x7F) << ShiftAmount;
                ShiftAmount += 0x7;
            } 
            while (Byte & 0x80);
            return Result;
        }

        std::string ReadString()
        {
            const std::uint8_t Byte = this->ReadType<std::uint8_t>();
            if (Byte != 0x0B)
                return "N/A";
            const std::uint64_t Size = this->ReadUleb128();
            std::string Buffer(Size, ' ');
            for (std::uint32_t i = 0; i < Size; i++) 
            {
                Buffer[i] = static_cast<char>(this->m_CurrentStream.get());
            }
            return Buffer;
        }

        template<typename T> 
        T ReadType()
        {
            T Value;
            this->m_CurrentStream.read(reinterpret_cast<char*>(&Value), sizeof(T));
            return Value;
        }

        void Seek(const std::int32_t& Amount)
        {
            this->m_CurrentStream.seekg(Amount, std::ios::cur);
        }

        std::ifstream& GetStream()
        {
            return this->m_CurrentStream;
        }

    private:
        std::ifstream m_CurrentStream;
    };
}