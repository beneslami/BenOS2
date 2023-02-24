#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/driver.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>

//#define GRAPHICSMODE 0

using namespace BenOS;
using namespace BenOS::common;
using namespace BenOS::drivers;
using namespace BenOS::hardwarecommunication;
using namespace BenOS::gui;

void printf(char *str){
	uint16_t* VideoMemory = (uint16_t *)0xb8000;
    static uint8_t x = 0, y = 0;
	for(int i = 0; str[i] != '\0'; ++i){
        switch (str[i]) {
            case '\n':
                x = 0;
                y++;
                break;
            case '\t':
                x += 7;
            default:
                VideoMemory[80*y + x] = (VideoMemory[80*y + x] & 0xFF00 | str[i]);
                x++;
                break;
        }
        if(x >= 80){
            y++;
            x = 0;
        }
        if(y >= 25){
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y + x] = (VideoMemory[80*y + x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
	}
}

void printfHex(uint8_t key){
    char *foo = "KEYBOARD 0x00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0x0F];
    foo[1] = hex[key & 0x0F];
    printf(foo);
}

class PrintKeyboardEventHandler : public KeyboardEventHandler{
public:
    void OnKeyDown(char c){
        char *foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler{
    int8_t x, y;
public:
    MouseToConsole(){

    }

    virtual void OnActivate(){
        uint16_t* VideoMemory = (uint16_t *)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*12+40] = ((VideoMemory[80*12+40] & 0xF000) << 4)
                                | ((VideoMemory[80*12+40] & 0x0F00) >> 4)
                                | ((VideoMemory[80*12+40] & 0x00FF));
    }

    void OnMouseMove(int xoffset, int yoffset){
        static uint16_t *VideoMemory = (uint16_t *) 0xb8000;
        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) << 4)
                                  | ((VideoMemory[80 * y + x] & 0x0F00) >> 4)
                                  | ((VideoMemory[80 * y + x] & 0x00FF));
        x += xoffset;
        if (x < 0) x = 0;
        if (x >= 88) x = 79;
        y -= yoffset;
        if (y < 0) y = 0;
        if (y >= 25) y = 24;
        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) << 4)
                                  | ((VideoMemory[80 * y + x] & 0x0F00) >> 4)
                                  | ((VideoMemory[80 * y + x] & 0x00FF));
    }
};

void taskA(){
    while(true){
        printf("Task 1\n");
    }
}

void taskB(){
    while(true){
        printf("task 2\n");
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors(){
	for(constructor *i = &start_ctors; i != &end_ctors; i++){
		(*i)();
	}
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber){
	printf("Hello. Welcome to the BenOS\n");
    printf("this is a simple OS running on VirtualBox\n");
	GlobalDescriptorTable gdt;
    TaskManager taskManager;
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    InterruptManager interrupts(0x20, &gdt, &taskManager);
    printf("Initializing Hardware, stage 1\n");
#ifdef GRAPHICSMODE
    Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
#endif
    DriverManager drvManager;

#ifdef GRAPHICSMODE
    KeyboardDriver keyboard(&interrupts, &desktop);
#else
    PrintKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
#endif
    drvManager.AddDriver(&keyboard);
#ifdef GRAPHICSMODE
    MouseDriver mouse(&interrupts, &desktop);
#else
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
#endif
    drvManager.AddDriver(&mouse);
    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);
#ifdef GRAPHICSMODE
    VideoGraphicsArray vga;
#endif
    printf("Initializing Hardware, stage 2\n");
    drvManager.ActivateAll();
    printf("Initializing Hardware, stage 3\n");
#ifdef GRAPHICSMODE
    vga.SetMode(320, 200, 8);
    Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
    desktop.AddChild(&win1);
    Window win2(&desktop, 40, 15, 30, 30, 0x00, 0xA8, 0x00);
    desktop.AddChild(&win2);
#endif

    interrupts.Activate();
	while(1){
#ifdef GRAPHICSMODE
        desktop.Draw(&vga);
#endif
    }
}
