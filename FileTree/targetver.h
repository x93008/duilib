#pragma once

// 如下宏定义要求的最低平台。要求的最低平台
// 是具有运行应用程序所需功能的 Windows、Internet Explorer 等产品的
// 最早版本。通过在指定版本及更早版本的平台上启用所有可用的功能，宏可以
// 正常工作。

// 如果必须要针对早于以下指定版本的平台，请修改下列定义。
// 有关不同平台对应值的最新信息，请参考 MSDN。
#ifndef WINVER                          // 指定要求的最低平台为 Windows Vista。
#define WINVER 0x0600           // 更改此值为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT            // 指定要求的最低平台为 Windows Vista。
#define _WIN32_WINNT 0x0600     // 更改此值为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINDOWS          // 指定要求的最低平台为 Windows 98。
#define _WIN32_WINDOWS 0x0410 // 更改此值为适当的值，以适用于 Windows Me 或更高版本。
#endif

#ifndef _WIN32_IE                       // 指定要求的最低平台为 Internet Explorer 7.0。
#define _WIN32_IE 0x0700        // 更改此值为相应的值，以适用于 IE 的其他版本。
#endif
