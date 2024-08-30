## 使用说明
1. 下载本工程及其子模块：
```bash
git clone https://github.com/ShadowThree/STM32V6_TouchGFX_template.git
git submodule update --init
```
2. 双击打开[`STM32V6_touchGFX_template.ioc`](./STM32V6_touchGFX_template.ioc)，并点击`GENERATE CODE`生成代码；
3. 双击打开`TouchGFX/ApplicationTemplate.touchgfx.part`，导入想要的`GUI`模板，并点击`Import`；然后就可以进行`UI`设计，代码生成，以及仿真了；
4. 打开[`Core/Inc/RTE_Components.h`](./Core/Inc/RTE_Components.h)文件（注意路径，工程中有两个这个文件），并加上如下内容：
```c
/* Keil::Compiler&ARM Compiler:I/O:STDOUT&User@1.2.0 */
#define RTE_Compiler_IO_STDOUT          /* Compiler I/O: STDOUT */
#define RTE_Compiler_IO_STDOUT_User     /* Compiler I/O: STDOUT User */
```
5. 双击`MDK-ARM/STM32V6_touchGFX_template.uvprojx`打开工程，编译下载运行；