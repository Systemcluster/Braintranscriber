//
// Braintranscriber
// Brainfuck and Ook! interpreter and translator
// Copyright (c) 2014 Christian Sdunek
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//

#include <iostream>
#include <cstring>
#include <vector>
#include <stack>

#include <fstream>
#include <streambuf>
#include <string>

int main(int argc, const char * argv[]) {
    enum modes {
        BF = 0x00000001,
        OO = 0x00000010,
        IN = 0x01000000,
        TR = 0x10000000,
    };
    int mode = BF | IN;
    std::string code;

    // No arguments given, just output info
    if(argc == 1) {
        std::cout << "Braintranscriber (1.2)\n";
        std::cout << "\n";
        std::cout << "bt [-options] [Brainfuck/Ook! code/path]\n";
        std::cout << "  -b\tinterpret Brainfuck (default)\n";
        std::cout << "  -o\tinterpret Ook!\n";
        std::cout << "  -i\trun interpreted code (default)\n";
        std::cout << "  -t\ttranslate Brainfuck to Ook or vice versa\n";
        std::cout << "  -f\tread code from file";
    }
    else {
        // Parse commandline arguments
        for(int a = 1; a < argc - 1; ++a) {
            if(!strcmp(argv[a], "-b"))
                mode = (mode | BF) & ~OO; else
            if(!strcmp(argv[a], "-o"))
                mode = (mode | OO) & ~BF; else
            if(!strcmp(argv[a], "-i"))
                mode = (mode | IN) & ~TR; else
            if(!strcmp(argv[a], "-t"))
                mode = (mode | TR) & ~IN; else
            if(!strcmp(argv[a], "-f")) {
                std::ifstream ifile(argv[argc-1]);
                if(!ifile) {
                    std::cout << "File not found: " << argv[argc-1] << std::endl;
                    return -1;
                }
                code = std::string{(std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>()};
                if(code.empty()) {
                    std::cout << "File empty: " << argv[argc-1] << std::endl;
                    return -1;
                }
            } else
            std::cout << "Unrecognized argument: " << argv[a] << std::endl;
        }
        if(code.empty()) {
            code = argv[argc - 1];
        }

        // Brainfuck mode
        if(mode & BF) {
            const char* pc {code.c_str()};
            const char* pm {code.c_str()+code.length()};
            // Translate Brainfuck to Ook!
            if(mode & TR) {
                for(; pc < pm; ++pc) {
                    switch(*pc) {
                        case '>': std::cout << "Ook. Ook? "; break;
                        case '<': std::cout << "Ook? Ook. "; break;
                        case '+': std::cout << "Ook. Ook. "; break;
                        case '-': std::cout << "Ook! Ook! "; break;
                        case '[': std::cout << "Ook! Ook? "; break;
                        case ']': std::cout << "Ook? Ook! "; break;
                        case '.': std::cout << "Ook! Ook. "; break;
                        case ',': std::cout << "Ook. Ook! "; break;
                        default: break;
                    }
                }
            }
            // Run Brainfuck program
            else if(mode & IN) {
                std::stack<const char*> stack;
                std::vector<char> mem (30000);
                char* mc = &(mem[0]);
                char rdbuf;
                for(; pc < pm; ++pc) {
                    switch(*pc) {
                        case '>':
                            ++mc;
                            if(mc>&(*(mem.rbegin()))) {
                                size_t pos {static_cast<size_t>(mc - &(mem[0]))/sizeof(char)};
                                mem.resize(mem.size()*2);
                                mc = &(mem[pos]);
                            }
                        break;
                        case '<':
                            if(mc>&(mem[0]))--mc;
                        break;
                        case '+':
                            ++(*mc);
                        break;
                        case '-':
                            --(*mc);
                        break;
                        case '[':
                            if(*mc) {
                                stack.push(pc);
                            }
                            else {
                                size_t in {0};
                                ++pc;
                                for(; *pc != ']' || in > 0; ++pc) {
                                    if(*pc == '[') ++in;
                                    else if(*pc == ']') --in;
                                }
                            }
                        break;
                        case ']':
                            if(*mc) pc = stack.top();
                            else stack.pop();
                        break;
                        case '.':
                            std::cout << *mc;
                        break;
                        case ',':
                            rdbuf = std::cin.get();
                            if(!std::cin.eof()) {
                                *mc = rdbuf;
                            }
                        break;
                        default: break;
                    }
                }
            }
        }

        // Ook! mode
        else if(mode & OO) {
            const char* pc {code.c_str()};
            const char* pm {code.c_str()+code.length()};
            // Translate Ook! to Brainfuck
            if(mode & TR) {
                for(; pc < pm; pc+=9) {
                    if(!strncmp("Ook. Ook?", pc, 9)) std::cout << '>'; else
                    if(!strncmp("Ook? Ook.", pc, 9)) std::cout << '<'; else
                    if(!strncmp("Ook. Ook.", pc, 9)) std::cout << '+'; else
                    if(!strncmp("Ook! Ook!", pc, 9)) std::cout << '-'; else
                    if(!strncmp("Ook! Ook?", pc, 9)) std::cout << '['; else
                    if(!strncmp("Ook? Ook!", pc, 9)) std::cout << ']'; else
                    if(!strncmp("Ook! Ook.", pc, 9)) std::cout << '.'; else
                    if(!strncmp("Ook. Ook!", pc, 9)) std::cout << ',';
                    else pc-=8;
                }
            }
            // Run Ook! program
            else if(mode & IN) {
                std::stack<const char*> stack;
                std::vector<char> mem (30000);
                char* mc = &(mem[0]);
                char rdbuf;
                for(; pc < pm; pc+=9) {
                    if(!strncmp("Ook. Ook?", pc, 9)) {
                        ++mc;
                        if(mc>&(*(mem.rbegin()))) {
                            size_t pos {static_cast<size_t>(mc - &(mem[0]))/sizeof(char)};
                            mem.resize(mem.size()*2);
                            mc = &(mem[pos]);
                        }
                    } else
                    if(!strncmp("Ook? Ook.", pc, 9)) {
                        if(mc>&(mem[0]))--mc;
                    } else
                    if(!strncmp("Ook. Ook.", pc, 9)) {
                        ++(*mc);
                    } else
                    if(!strncmp("Ook! Ook!", pc, 9)) {
                        --(*mc);
                    } else
                    if(!strncmp("Ook! Ook?", pc, 9)) {
                        if(*mc) {
                            stack.push(pc);
                        }
                        else {
                            size_t in {0};
                            pc+=9;
                            for(; strncmp(pc, "Ook? Ook!", 9) || in > 0; pc+=9) {
                                if(!strncmp(pc, "Ook? Ook!", 9)) ++in;
                                else if(!strncmp(pc, "Ook? Ook!", 9)) --in;
                                else if(strncmp("Ook. Ook?", pc, 9) &&
                                        strncmp("Ook? Ook.", pc, 9) &&
                                        strncmp("Ook. Ook.", pc, 9) &&
                                        strncmp("Ook! Ook!", pc, 9) &&
                                        strncmp("Ook! Ook.", pc, 9) &&
                                        strncmp("Ook. Ook!", pc, 9)) pc-=8;
                            }
                        }
                    } else
                    if(!strncmp("Ook? Ook!", pc, 9)) {
                        if(*mc) pc = stack.top();
                        else stack.pop();
                    } else
                    if(!strncmp("Ook! Ook.", pc, 9)) {
                        std::cout << *mc;
                    } else
                    if(!strncmp("Ook. Ook!", pc, 9)) {
                        rdbuf = std::cin.get();
                        if(!std::cin.eof()) {
                            *mc = rdbuf;
                        }
                    }
                    else pc-=8;
                }
            }
        }
    }

    std::cout << std::endl;
    return 0;
}
