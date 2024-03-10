//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <fstream>
#include <iostream>
#include <map>

#include "utils/FileSystem.h"

#define PACKER_MAGIC_NUMBER_FILE "spck"
#define PACKER_MAGIC_NUMBER_TOC "toc!"
#define PACKER_MAGIC_NUMBER_SIZE sizeof(uint32_t)
#define PACKER_FILE_VERSION 1

struct Example {
    int i;
    bool b;
    float f;
    int ia[10];
    bool ba[10];
    float fa[10];
};

union MagicNumber {
    const char* string;
    uint32_t number;
};

struct PackageHeader {
    MagicNumber magic;
    uint32_t version;
    uint64_t bodySize;
    uint64_t tocOffset;
};

struct TocEntry {
    uint32_t nameSize;
    uint32_t dataOffset;
    uint32_t dataSize;
    const char* name;

    uint32_t entrySize() const
    {
        return sizeof(TocEntry) + nameSize - 1;
    }
};

std::map<const char*, TocEntry*> entries;

const char* path = "/Users/j-mo/Desktop/example.pck";

void printHeader(const PackageHeader& h)
{
    std::cout << "Header" << "\n";
    std::cout << "magic: " << h.magic.string << "\n";
    std::cout << "version: " << h.version << "\n";
    std::cout << "bodySize: " << h.bodySize << "\n";
    std::cout << "tocOffset: " << h.tocOffset << "\n";
    std::cout << "\n";
}

void printToc(const TocEntry& t)
{
    std::cout << "ToC" << "\n";
    std::cout << "nameSize: " << t.nameSize << "\n";
    std::cout << "dataOffset: " << t.dataOffset << "\n";
    std::cout << "dataSize: " << t.dataSize << "\n";
    std::cout << "name: " << t.name << "\n";
    std::cout << "\n";
}

void printExample(const Example& e)
{
    std::cout << "Example" << "\n";
    std::cout << "i: " << e.i << "\n";
    std::cout << "b: " << e.b << "\n";
    std::cout << "f: " << e.f << "\n";

    std::cout << "ia: ";
    for (int i = 0; i < 10; ++i) std::cout << e.ia[i] << ", ";
    std::cout << "\n";

    std::cout << "ba: ";
    for (int i = 0; i < 10; ++i) std::cout << e.ba[i] << ", ";
    std::cout << "\n";

    std::cout << "fa: ";
    for (int i = 0; i < 10; ++i) std::cout << e.fa[i] << ", ";
    std::cout << "\n";
    std::cout << "\n";
}

int main()
{
    {
        PackageHeader header {
            {PACKER_MAGIC_NUMBER_FILE},
            PACKER_FILE_VERSION,
            sizeof(Example)*4+PACKER_MAGIC_NUMBER_SIZE+sizeof(TocEntry)+5+sizeof(TocEntry)+7,
            sizeof(Example)*4
        };

        std::vector<TocEntry> tv {
            {
                6,
             sizeof(Example)*2,
             sizeof(Example),
             "hello"
            },
            {
                8,
                sizeof(Example)*3,
                sizeof(Example),
                "hellone"
            }
        };

        Example e {};
        e.i = 7;
        e.b = true;
        e.f = 3.14;

        Example f {};
        f.i = 9;
        f.b = true;
        f.f = 0.01f;

        for (int i = 0; i < 10; ++i)
        {
            e.ia[i] = i;
            e.ba[i] = i%2;
            e.fa[i] = float(i) + (float(i)/10.f);

            f.ia[i] = i*2;
            f.ba[i] = !i%2;
            f.fa[i] = float(i) + (float(i)/5.f);
        }

        std::ofstream outputFileStream;
        outputFileStream.open(path, std::ios::out|std::ios::binary);
        outputFileStream.write(reinterpret_cast<char*>(&header), sizeof(PackageHeader));

        char zero[sizeof(Example)*2] = { 0 };
        outputFileStream.write(zero, sizeof(Example)*2);

        outputFileStream.write(reinterpret_cast<char*>(&e), sizeof(Example));

        outputFileStream.write(reinterpret_cast<char*>(&f), sizeof(Example));

        outputFileStream.write(PACKER_MAGIC_NUMBER_TOC, PACKER_MAGIC_NUMBER_SIZE);

        for (auto toc : tv)
        {
            outputFileStream.write(reinterpret_cast<char*>(&toc), toc.entrySize());
        }

        outputFileStream.close();
    }

    std::cout << "============================" << "\n";

    {
        PackageHeader header {};

        std::ifstream inputFileStream;
        inputFileStream.open(path, std::ios::in|std::ios::binary);
        inputFileStream.read(reinterpret_cast<char*>(&header), sizeof(PackageHeader));
        char* body = reinterpret_cast<char*>(malloc(header.bodySize));
        inputFileStream.read(body, header.bodySize);
        inputFileStream.close();

        printHeader(header);

        char* toc_start = body+header.tocOffset+PACKER_MAGIC_NUMBER_SIZE;
        char* toc_end = body+header.bodySize;
        char* toc_curr = toc_start;
        while (toc_curr < toc_end)
        {
            TocEntry* toc = reinterpret_cast<TocEntry*>(toc_curr);
            toc_curr += toc->entrySize();
            entries.emplace(toc->name, toc);
        }

        for (auto entry : entries)
        {
            TocEntry* toc = entry.second;
            printToc(*toc);
            Example* g = reinterpret_cast<Example*>(body+toc->dataOffset);
            printExample(*g);
        }

        free(body);
    }

    return 0;
}
