

### wahtis window 

پنجره یا همون window بخشی از GUI سیستم عامل است که وظیفش اینه که با منه کاربر interact بشه و ورودی هایی که من میدم رو بتونه بگیره و  کنترول کنه 


Types Of Window

- Main Window
- Child Window
- PopUp Window
- MDI Window


##### Whatis Visible Window

1. windows icon
2. windows menu
3. title bar
4. windows control buttons
5. borders
6. status bar
7. resize grips
8. size/location
9. style
10. exstyle
11. state
12. Visible


## پنجره در ویندوز — دید عمیق‌تر

### پنجره چیست؟

از دیدگاه سیستم‌عامل، هر پنجره یک **شیء کرنل** (kernel object) نیست، بلکه یک **آبجکت Win32** است که توسط `win32k.sys` مدیریت می‌شود. هر پنجره با یک **HWND** (Handle to Window) شناسایی می‌شود — یک عدد ۳۲/۶۴ بیتی که به یک ساختار داخلی در kernel اشاره می‌کند.

HWND  →  tagWND  (ساختار داخلی در win32k.sys)
          ├── style / exStyle
          ├── WNDPROC (آدرس تابع پردازش پیام)
          ├── موقعیت و سایز
          └── اطلاعات thread/process مالک


---

## انواع پنجره

### 1. Main Window (پنجره اصلی)
- بالاترین سطح در سلسله‌مراتب
- مستقیماً زیر Desktop قرار دارد
- با `CreateWindowEx` و style مثل `WS_OVERLAPPEDWINDOW` ساخته می‌شود
- هیچ parent ندارد (یا parent آن Desktop است)

### 2. Child Window (پنجره فرزند)
- **همیشه** داخل پنجره والد (parent) محصور است
- باید style ‍`WS_CHILD` داشته باشد
- خارج از محدوده parent رندر نمی‌شود (clipping)
- مثال: دکمه‌ها، TextBox‌ها، ListBox‌ها — همه child window هستند

### 3. Popup Window
- مثل child است اما **خارج از محدوده parent** می‌تواند نمایش داده شود
- style: `WS_POPUP`
- مثال: منوها، tooltip‌ها، MessageBox، dialog‌ها

### 4. MDI Window (Multiple Document Interface)
- معماری قدیمی‌تر برای داشتن چند پنجره درون یک پنجره اصلی
- یک پنجره `MDICLIENT` وجود دارد که child MDI windowها را مدیریت می‌کند
- امروزه بیشتر با Tabbed interface جایگزین شده

---

## اجزای Visible Window

### 1. Windows Icon
- آیکون کوچک (16x16) در گوشه چپ Title Bar
- از طریق `WM_GETICON` یا `SetClassLongPtr` تنظیم می‌شود
- دوبار کلیک روی آن → پنجره بسته می‌شود

### 2. Windows Menu (System Menu / Control Menu)
- منوی سیستمی که با راست‌کلیک روی Title Bar یا کلیک روی آیکون باز می‌شود
- شامل: Restore, Move, Size, Minimize, Maximize, Close
- با `GetSystemMenu` قابل دسترسی و تغییر است

### 3. Title Bar
- نوار بالایی که **Caption** پنجره را نمایش می‌دهد
- با `SetWindowText` تغییر می‌کند
- با `WS_CAPTION` style فعال می‌شود
- محل drag برای جابجایی پنجره (پیام `WM_NCHITTEST` → `HTCAPTION`)

### 4. Control Buttons
سه دکمه استاندارد:

| دکمه | پیام ارسالی | Style لازم |
|------|------------|------------|
| Minimize | `WM_SYSCOMMAND` + `SC_MINIMIZE` | `WS_MINIMIZEBOX` |
| Maximize/Restore | `WM_SYSCOMMAND` + `SC_MAXIMIZE` | `WS_MAXIMIZEBOX` |
| Close | `WM_SYSCOMMAND` + `SC_CLOSE` | `WS_SYSMENU` |

### 5. Borders
- لبه‌های پنجره که امکان resize را فراهم می‌کنند
- `WS_BORDER` → لبه ثابت (غیر قابل resize)
- `WS_THICKFRAME` (یا `WS_SIZEBOX`) → لبه قابل resize
- در NC area (Non-Client Area) قرار دارند

### 6. Status Bar
- نوار پایینی برای نمایش اطلاعات وضعیت
- یک **Child Window** جداگانه با class `msctls_statusbar32` است
- از طریق `CreateWindowEx` با این class name ساخته می‌شود

### 7. Resize Grips
- گوشه پایین-راست برای resize کردن با کشیدن
- معمولاً توسط Status Bar رندر می‌شود
- `WM_NCHITTEST` → `HTBOTTOMRIGHT`

### 8. Size / Location
- موقعیت: مختصات `(x, y)` نسبت به parent یا Screen
- سایز: `(width, height)` بر حسب پیکسل
- با `GetWindowRect` (مختصات صفحه) یا `GetClientRect` (فضای داخلی) خوانده می‌شود
- تفاوت مهم:

Window Rect  = کل پنجره شامل border و title bar
Client Rect  = فضای داخلی قابل رندر (بدون NC area)


### 9. Style
- یک عدد ۳۲ بیتی (bitmask) که ویژگی‌های پنجره را تعریف می‌کند
- با `GetWindowLongPtr(hwnd, GWL_STYLE)` خوانده می‌شود
- مهم‌ترین‌ها:

WS_VISIBLE      → پنجره قابل مشاهده است
WS_DISABLED     → ورودی کاربر قبول نمی‌کند
WS_CHILD        → پنجره فرزند
WS_POPUP        → پنجره popup
WS_OVERLAPPED   → پنجره اصلی معمولی


### 10. ExStyle (Extended Style)
- style تکمیلی با `GetWindowLongPtr(hwnd, GWL_EXSTYLE)`
- مثال‌های مهم:

WS_EX_TOPMOST       → همیشه روی بقیه پنجره‌ها
WS_EX_TRANSPARENT   → کلیک‌ها از پنجره رد می‌شوند
WS_EX_TOOLWINDOW    → در taskbar نمایش داده نمی‌شود
WS_EX_LAYERED       → امکان شفافیت (alpha blending)


### 11. State
وضعیت فعلی پنجره:

| State | مقدار | توضیح |
|-------|-------|-------|
| Normal | `SW_SHOWNORMAL` | اندازه معمولی |
| Minimized | `SW_MINIMIZE` | کوچک شده در taskbar |
| Maximized | `SW_MAXIMIZE` | تمام صفحه |
| Hidden | `SW_HIDE` | مخفی (HWND هنوز وجود دارد) |

با `GetWindowPlacement` یا `IsIconic` / `IsZoomed` بررسی می‌شود.

### 12. Visible
- فقط یعنی bit مربوط به `WS_VISIBLE` در style ست است
- با `IsWindowVisible(hwnd)` چک می‌شود
- نکته: یک پنجره می‌تواند `WS_VISIBLE` داشته باشد اما پشت پنجره دیگری مخفی باشد — این با **Visibility** متفاوت است

---

## سلسله‌مراتب پنجره‌ها

Desktop (HWND_DESKTOP)
    ├── Main Window (HWND_A)
    │       ├── Child: Button
    │       ├── Child: TextBox
    │       └── Child: StatusBar
    ├── Popup Window (HWND_B)
    └── Main Window (HWND_C)


همه این روابط با `GetParent`, `GetWindow`, `EnumChildWindows` قابل پیمایش است.



## روش‌های پیاده‌سازی پنجره در ویندوز

---

## 1. Win32 API (Native)

**پایه‌ای‌ترین روش** — تمام روش‌های دیگر در نهایت به این تبدیل می‌شوند.

### مراحل:
```c
// 1. ثبت Window Class
WNDCLASSEX wc = {0};
wc.cbSize = sizeof(WNDCLASSEX);
wc.lpfnWndProc = WindowProc;  // تابع پردازش پیام
wc.hInstance = hInstance;
wc.lpszClassName = "MyWindowClass";
wc.hCursor = LoadCursor(NULL, IDC_ARROW);
wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
RegisterClassEx(&wc);

// 2. ساخت پنجره
HWND hwnd = CreateWindowEx(
    0,                              // ExStyle
    "MyWindowClass",                // Class name
    "My Window",                    // Title
    WS_OVERLAPPEDWINDOW,            // Style
    CW_USEDEFAULT, CW_USEDEFAULT,   // موقعیت
    800, 600,                       // سایز
    NULL,                           // Parent
    NULL,                           // Menu
    hInstance,
    NULL
);

// 3. نمایش و به‌روزرسانی
ShowWindow(hwnd, SW_SHOW);
UpdateWindow(hwnd);

// 4. Message Loop
MSG msg;
while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);  // ارسال به WindowProc
}

// 5. Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            TextOut(hdc, 10, 10, "Hello Win32", 11);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
```

### ویژگی‌ها:
- کنترل کامل روی همه چیز
- بدون overhead
- کد زیاد و پیچیده برای UI ساده
- مناسب: سیستم‌های embedded، ابزارهای سطح پایین، performance-critical

---

## 2. MFC (Microsoft Foundation Classes)

**Wrapper شیءگرا** روی Win32 API — از دهه ۹۰ میلادی.

### مراحل:
```cpp
// 1. کلاس Application
class MyApp : public CWinApp {
public:
    BOOL InitInstance() override {
        CMainFrame* pFrame = new CMainFrame();
        pFrame->ShowWindow(SW_SHOW);
        pFrame->UpdateWindow();
        m_pMainWnd = pFrame;
        return TRUE;
    }
};
MyApp theApp;

// 2. کلاس Window (Frame)
class CMainFrame : public CFrameWnd {
public:
    CMainFrame() {
        Create(NULL, _T("MFC Window"),
               WS_OVERLAPPEDWINDOW,
               CRect(100, 100, 900, 700));
    }

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
};

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_PAINT()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

void CMainFrame::OnPaint() {
    CPaintDC dc(this);
    dc.TextOut(10, 10, _T("Hello MFC"));
}

void CMainFrame::OnDestroy() {
    PostQuitMessage(0);
}
```

### ساختار:
CWinApp (برنامه)
  └── CFrameWnd (پنجره اصلی)
        ├── CView (محتوا)
        ├── CToolBar (نوار ابزار)
        └── CStatusBar (نوار وضعیت)


### ویژگی‌ها:
- سینتکس ساده‌تر از Win32 خام
- پشتیبانی داخلی از Document/View architecture
- قدیمی و در حال deprecated شدن
- Visual Studio wizard برای setup
- مناسب: پروژه‌های legacy، سیستم‌های صنعتی قدیمی

---

## 3. CLR (Common Language Runtime) — WinForms / WPF

### 3.1. WinForms (.NET Wrapper)

**Managed wrapper** روی Win32 — هر `Control` یک HWND دارد.

```csharp
using System;
using System.Windows.Forms;

public class MyForm : Form {
    public MyForm() {
        Text = "WinForms Window";
        Size = new Size(800, 600);
        
        Button btn = new Button {
            Text = "Click Me",
            Location = new Point(10, 10)
        };
        btn.Click += (s, e) => MessageBox.Show("Hello WinForms");
        Controls.Add(btn);
    }
    
    protected override void OnPaint(PaintEventArgs e) {
        e.Graphics.DrawString("Hello", Font, Brushes.Black, 10, 50);
    }
}

static class Program {
    [STAThread]
    static void Main() {
        Application.EnableVisualStyles();
        Application.Run(new MyForm());
    }
}
```

### Call Stack در WinForms:
Button.OnClick()  →  (Managed C#)
   ↓
Control.WndProc()  →  (Managed wrapper)
   ↓
NativeWindow.Callback()  →  (P/Invoke bridge)
   ↓
DefWindowProc()  →  (user32.dll)


### 3.2. WPF (Windows Presentation Foundation)

**Rendering خود را با DirectX** انجام می‌دهد — بدون HWND برای هر control.

```xml
<!-- MainWindow.xaml -->
<Window x:Class="MyApp.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        Title="WPF Window" Height="600" Width="800">
    <Grid>
        <Button Content="Click Me" 
                Click="Button_Click"
                Width="100" Height="30"/>
        <TextBlock Text="Hello WPF" 
                   FontSize="20" 
                   Margin="10,50,0,0"/>
    </Grid>
</Window>
```

```csharp
// MainWindow.xaml.cs
public partial class MainWindow : Window {
    public MainWindow() {
        InitializeComponent();
    }
    
    private void Button_Click(object sender, RoutedEventArgs e) {
        MessageBox.Show("Hello WPF");
    }
}
```

### معماری WPF:
XAML (UI Definition)
   ↓
Visual Tree (logical elements)
   ↓
Composition Engine
   ↓
DirectX (GPU rendering)
   ↓
Desktop Window Manager (DWM)


### تفاوت WinForms vs WPF:

| ویژگی | WinForms | WPF |
|-------|---------|-----|
| Rendering | GDI+ (CPU) | DirectX (GPU) |
| HWND per control | بله | خیر (فقط پنجره اصلی) |
| Data Binding | محدود | کامل و قدرتمند |
| Styling | محدود | کامل (Themes, Templates) |
| Performance | کمتر | بیشتر (GPU accelerated) |
| Learning Curve | آسان | سخت‌تر |

---

## 4. OpenGL

**API گرافیکی cross-platform** — برای rendering 3D/2D.

### مراحل (با GLFW):
```c
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() {
    // 1. مقداردهی GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // 2. ساخت پنجره
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    // 3. مقداردهی GLEW
    glewInit();
    
    // 4. Loop رندرینگ
    while (!glfwWindowShouldClose(window)) {
        // پردازش ورودی
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        // رندرینگ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // رسم (مثلاً مثلث)
        // ... shader activation, VAO binding, glDrawArrays
        
        glfwSwapBuffers(window);  // Double buffering
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
```

### معماری OpenGL در ویندوز:
Your Code
   ↓
OpenGL API (opengl32.dll)
   ↓
ICD (Installable Client Driver) - درایور GPU
   ↓
Hardware (GPU)


### نکات:
- OpenGL **خودش پنجره نمی‌سازد** — نیاز به کتابخانه مثل GLFW/GLUT/SDL دارد
- برای rendering استفاده می‌شود، نه UI controls
- Cross-platform (ویندوز، لینوکس، مک)
- مناسب: بازی‌ها، شبیه‌سازی‌های علمی، نرم‌افزار CAD

---

## 5. DirectX (Direct3D)

**API گرافیکی مایکروسافت** — فقط ویندوز و Xbox.

### مراحل (Direct3D 11):
```cpp
#include <d3d11.h>
#include <dxgi.h>

// 1. ساخت پنجره با Win32
HWND hwnd = CreateWindowEx(...);

// 2. ساخت Device و SwapChain
DXGI_SWAP_CHAIN_DESC scd = {0};
scd.BufferCount = 1;
scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
scd.OutputWindow = hwnd;
scd.SampleDesc.Count = 1;
scd.Windowed = TRUE;

ID3D11Device* device;
ID3D11DeviceContext* context;
IDXGISwapChain* swapchain;

D3D11CreateDeviceAndSwapChain(
    NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
    NULL, 0, D3D11_SDK_VERSION,
    &scd, &swapchain, &device, NULL, &context
);

// 3. ساخت Render Target
ID3D11RenderTargetView* rtv;
ID3D11Texture2D* backbuffer;
swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbuffer);
device->CreateRenderTargetView(backbuffer, NULL, &rtv);
context->OMSetRenderTargets(1, &rtv, NULL);

// 4. Render Loop
MSG msg;
while (true) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    // رندرینگ
    float color[4] = {0.0f, 0.2f, 0.4f, 1.0f};
    context->ClearRenderTargetView(rtv, color);
    
    // رسم (vertex/pixel shaders, draw calls)
    // ...
    
    swapchain->Present(1, 0);  // VSync
}
```

### نسخه‌های DirectX:

| نسخه | سال | ویژگی کلیدی |
|------|-----|-------------|
| DirectX 9 | 2002 | Shader Model 3.0 |
| DirectX 10 | 2006 | Geometry Shader |
| DirectX 11 | 2009 | Tessellation, Compute Shaders |
| DirectX 12 | 2015 | Low-level API، کنترل بیشتر |

### DirectX 12 (مدرن):
```cpp
// بسیار low-level — کنترل دقیق روی GPU
// مدیریت دستی Command Lists، Descriptor Heaps
ID3D12CommandQueue* commandQueue;
ID3D12CommandAllocator* commandAllocator;
ID3D12GraphicsCommandList* commandList;

commandAllocator->Reset();
commandList->Reset(commandAllocator, pipelineState);
commandList->SetGraphicsRootSignature(rootSignature);
commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
commandList->DrawInstanced(3, 1, 0, 0);
commandList->Close();

commandQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&commandList);
swapChain->Present(1, 0);
```

### مقایسه OpenGL vs DirectX:

| ویژگی | OpenGL | DirectX |
|-------|--------|---------|
| Platform | Cross-platform | فقط Windows/Xbox |
| API Level | Higher-level (راحت‌تر) | Lower-level (DX12) |
| Performance | خوب | عالی (DX12) |
| Ecosystem | باز | بسته (Microsoft) |
| Tools | محدود | عالی (PIX, RenderDoc) |

---

## جمع‌بندی — کدام روش را انتخاب کنیم؟

Win32 Native → کنترل کامل، بدون dependency، سخت
MFC          → Legacy، نه برای پروژه جدید
WinForms     → UI ساده، سریع، .NET
WPF          → UI مدرن، GPU-accelerated، MVVM
OpenGL       → Cross-platform 3D/2D rendering
DirectX      → بهترین performance در ویندوز، بازی‌ها


### Decision Tree:
نیاز به UI ساده؟
   ├─ بله → .NET (WinForms / WPF)
   └─ خیر → نیاز به 3D rendering؟
          ├─ بله → OpenGL (cross-platform) یا DirectX (ویندوز)
          └─ خیر → کنترل کامل لازم؟
                 ├─ بله → Win32 Native
                 └─ خیر → WPF



##### user interaction in GUI application

1. window
2. form
3. MDI
4. modal dialog
5. modeless dialog
6. task dialog


## انواع تعامل کاربر در برنامه‌های GUI

---

## 1. Window (پنجره)

**پایه‌ای‌ترین واحد رابط کاربری** — هر چیزی که روی صفحه نمایش داده می‌شود در نهایت یک Window است.

- یک آبجکت Win32 با `HWND` که توسط `win32k.sys` مدیریت می‌شود.
- می‌تواند main، child یا popup باشد.
- ساخته شده با `CreateWindowEx` و دارای `WindowProc` برای پردازش پیام‌ها.

```c
HWND hwnd = CreateWindowEx(0, "MyClass", "Title",
    WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, hInst, NULL);
```

از دیدگاه فنی، **همه‌ی موارد بعدی (Form، Dialog، MDI) در واقع نوعی Window هستند** که با استایل‌ها و رفتارهای خاص ساخته می‌شوند.

---

## 2. Form (فرم)

**انتزاع سطح بالاتر** از Window در فریم‌ورک‌های مدیریت‌شده (.NET).

- در WinForms کلاس `System.Windows.Forms.Form`، در WPF کلاس `Window`.
- یک container برای controls (دکمه، تکست‌باکس، لیبل و...).
- معمولاً یک پنجره‌ی top-level با Title Bar و border کامل.
- زیر سرپوش، یک HWND واقعی دارد (در WinForms) یا یک HWND ریشه با rendering داخلی DirectX (در WPF).

```csharp
public class MyForm : Form {
    public MyForm() {
        Text = "My Form";
        Controls.Add(new Button { Text = "OK" });
    }
}
```

تفاوت کلیدی Form با Window خام: Form مدیریت چرخه‌ی حیات، event handling و layout را به‌صورت managed انجام می‌دهد.

---

## 3. MDI (Multiple Document Interface)

**معماری چندسندی** — نمایش چندین سند داخل یک پنجره‌ی اصلی.

### ساختار سه‌لایه:
MDI Frame Window (پنجره اصلی)
   └── MDI Client Window (کلاس "MDICLIENT" — ناحیه‌ی محتوا)
         ├── MDI Child 1 (سند اول)
         ├── MDI Child 2 (سند دوم)
         └── MDI Child 3 (سند سوم)


### مشخصات:
- **Frame** با استایل معمولی ساخته می‌شود؛ یک MDI Client به‌صورت خودکار به آن اضافه می‌شود.
- **Child**ها با `WS_EX_MDICHILD` و پیام `WM_MDICREATE` ساخته می‌شوند.
- هر child قابل minimize/maximize/restore داخل ناحیه‌ی client است.
- منوی Window برای جابجایی بین سندها (Cascade, Tile).

```c
// ساخت MDI Client
CLIENTCREATESTRUCT ccs;
ccs.hWindowMenu = GetSubMenu(hMenu, WINDOW_MENU_POS);
ccs.idFirstChild = ID_MDI_FIRSTCHILD;
HWND hwndClient = CreateWindowEx(0, "MDICLIENT", NULL,
    WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
    0, 0, 0, 0, hwndFrame, NULL, hInst, &ccs);
```

### وضعیت امروز:
معماری **قدیمی و توصیه‌نشده** است. جایگزین مدرن: **Tabbed Document Interface (TDI)** مانند مرورگرها یا IDEها. مثال کلاسیک MDI: نسخه‌های قدیمی Microsoft Word و Excel.

---

## 4. Modal Dialog (دیالوگ وضعیتی/انسدادی)

**پنجره‌ای که تا بسته نشود، تعامل با پنجره‌ی والد را مسدود می‌کند.**

### رفتار:
- کاربر **مجبور است** قبل از ادامه، با دیالوگ تعامل کند (OK/Cancel).
- پنجره‌ی والد disable می‌شود تا دیالوگ بسته شود.
- اجرای کد در نقطه‌ی فراخوانی **متوقف (block)** می‌شود تا نتیجه برگردد.

```c
// با DialogBox — تا بسته شدن بلاک می‌شود
INT_PTR result = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG),
                           hwndParent, DialogProc);
// کد اینجا فقط بعد از بسته شدن دیالوگ اجرا می‌شود
```

```csharp
// در .NET
DialogResult result = myDialog.ShowDialog();  // بلاک می‌شود
if (result == DialogResult.OK) { /* ... */ }
```

### مثال‌ها:
`MessageBox`, دیالوگ Open/Save File، Print Dialog، پنجره‌ی Settings که باید قبل از ادامه بسته شود.

### نکته‌ی فنی:
modality واقعی با **disable کردن owner window** و یک **message loop داخلی** پیاده‌سازی می‌شود — نه با thread جداگانه.

---

## 5. Modeless Dialog (دیالوگ غیرانسدادی)

**پنجره‌ای مستقل که والد را مسدود نمی‌کند** — کاربر می‌تواند هم‌زمان با هر دو کار کند.

### رفتار:
- پنجره‌ی والد همچنان **فعال و قابل تعامل** باقی می‌ماند.
- اجرای کد **بلاک نمی‌شود** — بلافاصله ادامه می‌یابد.
- باید پیام‌های دیالوگ در message loop اصلی مدیریت شوند (`IsDialogMessage`).

```c
// با CreateDialog — بلاک نمی‌شود
HWND hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_FINDDLG),
                         hwndParent, DialogProc);
ShowWindow(hDlg, SW_SHOW);
// کد بلافاصله ادامه می‌یابد

// در message loop:
while (GetMessage(&msg, NULL, 0, 0)) {
    if (!IsDialogMessage(hDlg, &msg)) {  // مدیریت دیالوگ
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
```

```csharp
// در .NET
myDialog.Show();  // بلاک نمی‌شود
```

### مثال‌ها:
دیالوگ **Find & Replace** (می‌توانید هم‌زمان متن را ویرایش کنید)، پنل‌های Toolbox، Properties Window در IDE.

---

## مقایسه‌ی Modal vs Modeless

| ویژگی | Modal | Modeless |
|-------|-------|----------|
| مسدودسازی والد | بله | خیر |
| بلاک شدن کد | بله | خیر |
| تابع ساخت | `DialogBox` / `ShowDialog` | `CreateDialog` / `Show` |
| message loop | داخلی (مجزا) | اصلی برنامه (`IsDialogMessage`) |
| کاربرد | تصمیم اجباری کاربر | ابزار جانبی هم‌زمان |
| مثال | MessageBox، Save As | Find & Replace |

---

## 6. Task Dialog (دیالوگ وظیفه)

**نسل مدرن MessageBox** — معرفی‌شده از Windows Vista در `comctl32.dll` (نسخه 6+).

### مزایا نسبت به MessageBox:
- دکمه‌های سفارشی (command links).
- آیکون‌های بزرگ و سفارشی.
- متن اصلی (main instruction) + متن توضیحی + footer.
- نوار پیشرفت (progress bar).
- چک‌باکس ("Don't show again").
- ناحیه‌ی قابل گسترش (expandable details).
- callback برای رویدادها.

### دو API موجود:
```c
// 1. ساده — TaskDialog
TaskDialog(hwndParent, hInst,
    L"Title", L"Main Instruction",
    L"Additional content text...",
    TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON,
    TD_INFORMATION_ICON, &button);

// 2. کامل و قابل تنظیم — TaskDialogIndirect
TASKDIALOGCONFIG config = {0};
config.cbSize = sizeof(config);
config.hwndParent = hwndParent;
config.dwFlags = TDF_USE_COMMAND_LINKS | TDF_ENABLE_HYPERLINKS;
config.pszMainInstruction = L"آیا می‌خواهید ذخیره کنید؟";
config.pszContent = L"تغییرات ذخیره‌نشده از بین خواهند رفت.";

TASKDIALOG_BUTTON buttons[] = {
    { 1001, L"ذخیره\nتغییرات را در دیسک می‌نویسد" },
    { 1002, L"عدم ذخیره\nتغییرات را دور می‌اندازد" },
};
config.pButtons = buttons;
config.cButtons = ARRAYSIZE(buttons);

int clicked;
TaskDialogIndirect(&config, &clicked, NULL, NULL);
```

### نکته‌ی مهم:
نیازمند **manifest** برای استفاده از `comctl32.dll` نسخه 6.0 است؛ در غیر این صورت تابع fail می‌شود:
```xml
<dependency>
  <dependentAssembly>
    <assemblyIdentity type="win32" name="Microsoft.Windows.Common-Controls"
        version="6.0.0.0" processorArchitecture="*"
        publicKeyToken="6595b64144ccf1df" language="*"/>
  </dependentAssembly>
</dependency>
```

---

## جمع‌بندی سلسله‌مراتب مفهومی

Window (پایه — HWND)
  │
  ├── Form (انتزاع managed با controls)
  │
  ├── MDI (چند سند در یک frame)
  │
  └── Dialog (پنجره‌ی تعاملی موقت)
        ├── Modal     → مسدودکننده، تصمیم اجباری
        ├── Modeless  → مستقل، تعامل هم‌زمان
        └── Task Dialog → نسل مدرن، غنی و انعطاف‌پذیر


### از دیدگاه امنیتی/تحلیلی (مرتبط با کار شما):
- همه‌ی این‌ها در نهایت به `CreateWindowEx` → `NtUserCreateWindowEx` → `win32k.sys` می‌رسند.
- **Modal dialogها** به‌دلیل message loop داخلی، گاهی برای تشخیص injection یا UI hijacking جالب‌اند.
- **Task Dialogها** را می‌توان با هوک `TaskDialogIndirect` در `comctl32.dll` ردگیری کرد.
- شناسایی نوع پنجره از طریق استایل‌ها (`GWL_STYLE`/`GWL_EXSTYLE`) و کلاس پنجره (`#32770` کلاس استاندارد dialogها) امکان‌پذیر است.


خلاصه بخواهیم در نظر بگیریم MDI مثلا یه برنامه رو حستب کنید که این برنامه parent چندین child داره که هرکدوم یه کاری رو انجام میدن این میشه MDI


##### پنجره بعدی میشه Dialog  : به عنوان مثال  من  notepad باز میکنم بعدش کلید ctrl + O رو میزنم 

![[Pasted image 20260626153439.png]]

همونطور که مشاهده میکنید یک صفحه جدید ایجاد میشه که برای یک task خاص هست در نظر گرفه میشه اما نکتش چیه، notepad من فریز شده چرا چون یک window به صورت modaldialog ایجاد شده نه modeless dialog هستش 


![[Pasted image 20260626154526.png]]

اینم حالت modeless dialog هست مثلا من میتونم در کنار command تو windbg ابزار های دیگری که زیر مجموعش هست رو بیارم بالا و باهاش کار کنم



## WNDPROC (Window Procedure)

**قلب مرکزی پردازش پیام‌ها** در معماری Win32 — تابعی که تمام eventها و پیام‌های یک پنجره را دریافت و پردازش می‌کند.

---

## تعریف و امضا

```c
LRESULT CALLBACK WindowProc(
    HWND   hwnd,      // handle پنجره‌ای که پیام برای آن است
    UINT   uMsg,      // شناسه پیام (WM_*)
    WPARAM wParam,    // پارامتر اول (معنای متغیر بسته به پیام)
    LPARAM lParam     // پارامتر دوم (معنای متغیر بسته به پیام)
);
```

**نوع بازگشتی:** `LRESULT` (معمولاً `0` برای موفقیت، بسته به پیام متفاوت)

**Calling convention:** `CALLBACK` (معادل `__stdcall` در x86)

---

## نقش WNDPROC

هر پنجره **حتماً** یک Window Procedure دارد که:

1. **تمام پیام‌های ارسال‌شده به پنجره را دریافت می‌کند**
2. **منطق پاسخ به هر رویداد را پیاده‌سازی می‌کند**
3. **کنترل کامل رفتار پنجره را در اختیار دارد**

بدون WNDPROC، پنجره به هیچ‌چیز پاسخ نمی‌دهد — نه کلیک، نه کیبورد، نه بسته شدن.

---

## ثبت WNDPROC در Window Class

قبل از ساخت پنجره، باید یک **Window Class** ثبت کنید که WNDPROC را مشخص می‌کند:

```c
WNDCLASSEX wc = {0};
wc.cbSize        = sizeof(WNDCLASSEX);
wc.lpfnWndProc   = WindowProc;  // ← اشاره‌گر به تابع پردازش‌گر
wc.hInstance     = hInstance;
wc.lpszClassName = L"MyWindowClass";
wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

if (!RegisterClassEx(&wc)) {
    // خطا در ثبت کلاس
}
```

سپس پنجره با این کلاس ساخته می‌شود:

```c
HWND hwnd = CreateWindowEx(0, L"MyWindowClass", L"Title",
    WS_OVERLAPPEDWINDOW, x, y, w, h,
    NULL, NULL, hInstance, NULL);
```

---

## پیاده‌سازی نمونه WNDPROC

```c
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            // پنجره تازه ساخته شد
            // lParam اشاره‌گر به CREATESTRUCT دارد
            return 0;

        case WM_PAINT:
        {
            // نیاز به رسم محتوا
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // رسم کردن با GDI
            TextOut(hdc, 10, 10, L"Hello, World!", 13);
            
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_SIZE:
            // اندازه پنجره تغییر کرد
            // LOWORD(lParam) = عرض جدید
            // HIWORD(lParam) = ارتفاع جدید
            return 0;

        case WM_LBUTTONDOWN:
            // کلیک چپ موس
            // LOWORD(lParam) = x
            // HIWORD(lParam) = y
            MessageBox(hwnd, L"Clicked!", L"Info", MB_OK);
            return 0;

        case WM_KEYDOWN:
            // کلید فشرده شد
            // wParam = کد مجازی کلید (VK_*)
            if (wParam == VK_ESCAPE) {
                PostQuitMessage(0);
            }
            return 0;

        case WM_CLOSE:
            // کاربر دکمه X را زد
            if (MessageBox(hwnd, L"آیا می‌خواهید خارج شوید?",
                          L"تأیید", MB_OKCANCEL) == IDOK) {
                DestroyWindow(hwnd);
            }
            return 0;

        case WM_DESTROY:
            // پنجره در حال نابودی است
            PostQuitMessage(0);  // خروج از message loop
            return 0;

        default:
            // پیام‌های پردازش‌نشده را به handler پیش‌فرض بفرست
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
```

---

## جریان اجرای پیام

[رویداد سیستم] → win32k.sys
         ↓
    User32.dll (message queue)
         ↓
    GetMessage/PeekMessage
         ↓
    TranslateMessage (تبدیل WM_KEYDOWN به WM_CHAR)
         ↓
    DispatchMessage
         ↓
    ═══════════════════════════════
    ║   WNDPROC شما فراخوانی می‌شود   ║
    ═══════════════════════════════
         ↓
    بازگشت به message loop


---

## انواع پیام‌ها (Message Categories)

### 1. **Window Management**
- `WM_CREATE` — پنجره در حال ساخت است
- `WM_DESTROY` — پنجره در حال نابودی است
- `WM_CLOSE` — درخواست بسته شدن (قابل لغو)
- `WM_ACTIVATE` — پنجره فعال/غیرفعال شد
- `WM_SETFOCUS` / `WM_KILLFOCUS` — فوکوس گرفت/از دست داد

### 2. **Painting & Drawing**
- `WM_PAINT` — باید محتوا رسم شود
- `WM_ERASEBKGND` — پس‌زمینه باید پاک شود
- `WM_NCPAINT` — ناحیه non-client (border, title bar) باید رسم شود

### 3. **Input**
- **Mouse:** `WM_LBUTTONDOWN`, `WM_MOUSEMOVE`, `WM_MOUSEWHEEL`
- **Keyboard:** `WM_KEYDOWN`, `WM_KEYUP`, `WM_CHAR`, `WM_SYSKEYDOWN`

### 4. **Size & Position**
- `WM_SIZE` — اندازه تغییر کرد
- `WM_MOVE` — موقعیت تغییر کرد
- `WM_SIZING` — کاربر در حال resize است (real-time)
- `WM_MOVING` — کاربر در حال جابجایی است

### 5. **System**
- `WM_COMMAND` — منو یا دکمه زده شد
- `WM_NOTIFY` — control فرزند اطلاع‌رسانی می‌کند
- `WM_TIMER` — تایمر فعال شد
- `WM_SYSCOMMAND` — منوی سیستمی (minimize, maximize, close)

---

## DefWindowProc — پردازش‌گر پیش‌فرض

**هر پیامی که شما پردازش نمی‌کنید، باید به `DefWindowProc` فرستاده شود:**

```c
return DefWindowProc(hwnd, uMsg, wParam, lParam);
```

این تابع رفتار پیش‌فرض پنجره را پیاده‌سازی می‌کند:
- رسم Title Bar و Border
- مدیریت کلیدهای میانبر سیستمی (Alt+F4)
- مدیریت منوی سیستمی
- resize و move کردن پنجره
- و صدها رفتار دیگر

**اگر `DefWindowProc` را صدا نزنید، پنجره رفتار غیرمنتظره‌ای خواهد داشت.**

---

## Subclassing — جایگزینی WNDPROC

می‌توانید WNDPROC یک پنجره موجود را **جایگزین** کنید تا رفتار آن را تغییر دهید:

```c
// ذخیره WNDPROC قبلی
WNDPROC oldWndProc = (WNDPROC)SetWindowLongPtr(hwnd,
    GWLP_WNDPROC, (LONG_PTR)MyNewWndProc);

// WNDPROC جدید
LRESULT CALLBACK MyNewWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    // منطق سفارشی
    if (msg == WM_LBUTTONDOWN) {
        MessageBox(hwnd, L"Intercepted!", L"Hook", MB_OK);
    }
    
    // فراخوانی WNDPROC قبلی
    return CallWindowProc(oldWndProc, hwnd, msg, wp, lp);
}
```

**کاربرد:**
- تغییر رفتار controlهای استاندارد (دکمه، تکست‌باکس)
- اضافه کردن قابلیت‌های سفارشی
- logging یا monitoring پیام‌ها

**نکته امنیتی:** این تکنیک برای **hooking و injection** در malware و EDR bypass رایج است.

---

## Dialog Procedure (DlgProc) — نسخه خاص WNDPROC

برای dialogها، امضا کمی متفاوت است:

```c
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
            // دیالوگ در حال مقداردهی است
            return TRUE;  // TRUE = سیستم فوکوس را تنظیم کند

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
    }
    return FALSE;  // FALSE = پیام پردازش نشد
}
```

**تفاوت‌های کلیدی:**
- بازگشت `TRUE`/`FALSE` به‌جای `LRESULT`
- **هیچ‌وقت `DefWindowProc` صدا نمی‌زنید** — سیستم خودش مدیریت می‌کند
- استفاده از `EndDialog` برای بسته شدن (نه `DestroyWindow`)

---

## Window Messages: Sent vs Posted

### Sent Messages (همزمان)
```c
SendMessage(hwnd, WM_PAINT, 0, 0);
// ← کد اینجا بلاک می‌شود تا WNDPROC پیام را پردازش کند
```

**جریان:**
SendMessage → مستقیماً WNDPROC را صدا می‌زند → منتظر بازگشت می‌ماند


### Posted Messages (غیرهمزمان)
```c
PostMessage(hwnd, WM_USER+1, 0, 0);
// ← بلافاصله برمی‌گردد، پیام در صف قرار می‌گیرد
```

**جریان:**
PostMessage → پیام را به صف اضافه می‌کند → برمی‌گردد
              ↓
       (بعداً) GetMessage → DispatchMessage → WNDPROC


---

## از دیدگاه امنیتی و تحلیل (مرتبط با کار شما)

### 1. **WNDPROC Hook — تکنیک injection رایج**

```c
// Hooking برای ثبت keystrokeها (keylogger)
LRESULT CALLBACK HookedWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    if (msg == WM_KEYDOWN) {
        LogKeyToFile(wp);  // ثبت کلید
    }
    return CallWindowProc(originalProc, hwnd, msg, wp, lp);
}

SetWindowLongPtr(targetHwnd, GWLP_WNDPROC, (LONG_PTR)HookedWndProc);
```

### 2. **ردگیری از طریق Stack Walking**

وقتی WNDPROC فراخوانی می‌شود، call stack به این شکل است:

[0] MyWndProc
[1] user32!DispatchMessageW
[2] MyApp!WinMain (message loop)
[3] MyApp!__tmainCRTStartup
[4] kernel32!BaseThreadInitThunk
[5] ntdll!RtlUserThreadStart


**اگر hooking وجود داشته باشد:**
[0] HookedWndProc          ← آدرس مشکوک (خارج از ماژول اصلی)
[1] OriginalWndProc
[2] user32!DispatchMessageW


می‌توانید با `RtlCaptureStackBackTrace` این را شناسایی کنید.

### 3. **Integrity Check — تشخیص Subclassing**

```c
// خواندن WNDPROC فعلی
WNDPROC currentProc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);

// بررسی اینکه آیا در ماژول مورد انتظار است
HMODULE hMod;
GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                  (LPCTSTR)currentProc, &hMod);

if (hMod != hExpectedModule) {
    // احتمال hooking یا injection!
}
```

### 4. **Message Injection**

مهاجم می‌تواند با `SendMessage`/`PostMessage` پیام‌های جعلی ارسال کند:

```c
// فرستادن کلیک جعلی
PostMessage(targetHwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
PostMessage(targetHwnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
```

**دفاع:** استفاده از `UIPI` (User Interface Privilege Isolation) در Vista+ که پیام‌های cross-integrity-level را مسدود می‌کند.

---

## جمع‌بندی

WNDPROC = قلب تپنده هر پنجره Win32

┌─────────────────────────────────────┐
│  تمام رویدادها از اینجا عبور می‌کنند   │
│  • Mouse, Keyboard, Paint           │
│  • Size, Move, Focus                │
│  • System commands                  │
└─────────────────────────────────────┘
           ↓
    منطق برنامه شما
           ↓
    DefWindowProc (پیش‌فرض)


**نکات کلیدی:**
- هر پنجره دقیقاً یک WNDPROC دارد
- پیام‌های پردازش‌نشده باید به `DefWindowProc` فرستاده شوند
- Subclassing برای تغییر رفتار پنجره‌های موجود
- از دیدگاه امنیتی، نقطه‌ی حساس برای hooking و injection است



## hInstance — شناسه نمونه برنامه

`HINSTANCE`
یک **handle به module** است که کد اجرایی برنامه یا DLL را در حافظه نشان می‌دهد.

---

## تعریف دقیق

```c
typedef HANDLE HINSTANCE;  // در واقع یک void*
```

**در عمل:** `HINSTANCE` آدرس پایه (base address) فایل EXE یا DLL در حافظه است.

hInstance = 0x00400000  ← آدرس شروع PE image در حافظه


---

## چرا نیاز داریم؟

ویندوز باید بداند:
1. **کدام برنامه در حال اجرا است** (اگر چند نمونه از یک EXE داشته باشیم)
2. **منابع (Resources) کجا هستند** — آیکون‌ها، منوها، دیالوگ‌ها، رشته‌ها همه درون EXE/DLL ذخیره می‌شوند
3. **Window Class‌ها به کدام module تعلق دارند**

---

## دریافت hInstance

### در WinMain
```c
int WINAPI WinMain(
    HINSTANCE hInstance,      // ← سیستم‌عامل این را می‌دهد
    HINSTANCE hPrevInstance,  // ← همیشه NULL (قدیمی، Win16)
    LPSTR     lpCmdLine,
    int       nCmdShow
)
{
    // hInstance حالا در دسترس است
}
```

### در هر نقطه از برنامه
```c
// دریافت hInstance خود برنامه
HINSTANCE hInst = GetModuleHandle(NULL);

// دریافت hInstance یک DLL خاص
HINSTANCE hUser32 = GetModuleHandle(L"user32.dll");

// دریافت module یک آدرس حافظه
HMODULE hMod;
GetModuleHandleEx(
    GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
    (LPCTSTR)&SomeFunction,
    &hMod
);
```

---

## کاربردهای hInstance

### 1. **ثبت Window Class**
```c
WNDCLASSEX wc = {0};
wc.cbSize        = sizeof(WNDCLASSEX);
wc.lpfnWndProc   = WindowProc;
wc.hInstance     = hInstance;  // ← مشخص می‌کند این کلاس به کدام module تعلق دارد
wc.lpszClassName = L"MyClass";

RegisterClassEx(&wc);
```

**چرا لازم است؟** چون چند DLL ممکن است window classهای با نام یکسان ثبت کنند. `hInstance` آنها را تمایز می‌دهد:
DLL_A : "Button" → hInstance = 0x10000000
DLL_B : "Button" → hInstance = 0x20000000


### 2. **بارگذاری منابع (Resources)**
```c
// بارگذاری آیکون از منابع EXE
HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));

// بارگذاری منو
HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAINMENU));

// بارگذاری dialog template
DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT), hwndParent, AboutDlgProc);

// بارگذاری رشته
WCHAR szText[256];
LoadString(hInstance, IDS_HELLO, szText, 256);
```

**بدون `hInstance`:** سیستم نمی‌داند باید از کدام فایل EXE/DLL منابع را بخواند.

### 3. **ساخت پنجره**
```c
HWND hwnd = CreateWindowEx(
    0,
    L"MyClass",
    L"Title",
    WS_OVERLAPPEDWINDOW,
    x, y, w, h,
    NULL,
    NULL,
    hInstance,  // ← مشخص می‌کند window class از کدام module است
    NULL
);
```

### 4. **بارگذاری DLL دستی**
```c
// بارگذاری یک DLL
HINSTANCE hDll = LoadLibrary(L"mydll.dll");

if (hDll) {
    // دریافت آدرس یک تابع از DLL
    typedef int (*FuncPtr)(int);
    FuncPtr MyFunc = (FuncPtr)GetProcAddress(hDll, "MyFunction");
    
    if (MyFunc) {
        int result = MyFunc(42);
    }
    
    FreeLibrary(hDll);  // آزادسازی
}
```

---

## hInstance vs HMODULE

```c
typedef HINSTANCE HMODULE;  // در واقع یکی هستند!
```

**تفاوت مفهومی:**
- `HINSTANCE` → زمینه برنامه (application context)
- `HMODULE` → زمینه ماژول (module/library context)

**در عمل:** تفاوت معنایی ندارند، هر دو یک آدرس پایه هستند.

```c
HINSTANCE hInst = GetModuleHandle(NULL);
HMODULE   hMod  = GetModuleHandle(NULL);
// hInst == hMod → TRUE
```

---

## ساختار حافظه — چرا hInstance آدرس پایه است؟

وقتی یک EXE یا DLL لود می‌شود:

Disk:                        Memory:
┌─────────────────┐         ┌─────────────────┐
│  myapp.exe      │  Load   │ 0x00400000      │ ← hInstance
│  ├─ .text       │  ────→  │  DOS Header     │
│  ├─ .data       │         │  PE Header      │
│  ├─ .rsrc       │         │  .text section  │
│  └─ ...         │         │  .data section  │
└─────────────────┘         │  .rsrc section  │
                            └─────────────────┘


**hInstance = 0x00400000** (آدرس شروع DOS Header در حافظه)

APIهای بارگذاری منابع این آدرس را می‌گیرند و offset منبع را اضافه می‌کنند:
```c
// مثال ساده‌شده
HICON LoadIcon(HINSTANCE hInst, LPCWSTR name) {
    BYTE* base = (BYTE*)hInst;  // 0x00400000
    DWORD offset = FindResourceOffset(name);  // مثلاً 0x5000
    ICONDATA* pIcon = (ICONDATA*)(base + offset);  // 0x00405000
    return CreateIconFromData(pIcon);
}
```

---

## مثال کامل — استفاده از hInstance

```c
#include <windows.h>

// Resource IDs (در resource.h تعریف شده)
#define IDI_APPICON  101
#define IDR_MAINMENU 201
#define IDS_HELLO    301

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR cmd, int show)
{
    // 1. ثبت Window Class با hInstance
    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = L"MyApp";
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));  // بارگذاری آیکون
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    if (!RegisterClassEx(&wc)) return 1;
    
    // 2. ساخت پنجره با hInstance
    HWND hwnd = CreateWindowEx(
        0, L"MyApp", L"My Application",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, 
        LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAINMENU)),  // بارگذاری منو
        hInstance,  // ← اینجا
        NULL
    );
    
    if (!hwnd) return 1;
    
    ShowWindow(hwnd, show);
    UpdateWindow(hwnd);
    
    // 3. بارگذاری رشته از resources
    WCHAR szHello[256];
    LoadString(hInstance, IDS_HELLO, szHello, 256);
    MessageBox(hwnd, szHello, L"Greeting", MB_OK);
    
    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
```

---

## چند نمونه (Multiple Instances) از یک برنامه

اگر `myapp.exe` را دوبار اجرا کنید:

Process 1:                    Process 2:
┌──────────────────┐         ┌──────────────────┐
│ myapp.exe        │         │ myapp.exe        │
│ hInstance =      │         │ hInstance =      │
│   0x00400000     │         │   0x00A10000     │ ← آدرس متفاوت (ASLR)
└──────────────────┘         └──────────────────┘


**هر process حافظه جداگانه دارد** → `hInstance` آنها متفاوت است حتی اگر از یک EXE باشند.

---

## hPrevInstance — چرا همیشه NULL است؟

```c
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, ...)
```

**در Windows 16-bit (Win 3.x):**
- اگر برنامه قبلاً اجرا شده بود، `hPrevInstance` غیر-NULL بود
- برنامه‌ها می‌توانستند منابع نمونه قبلی را share کنند

**در Windows 32-bit و بعد (Win95+):**
- هر process حافظه جداگانه دارد
- `hPrevInstance` **همیشه NULL** است
- این پارامتر فقط برای سازگاری باقی مانده

**کد صحیح:**
```c
if (hPrevInstance) {
    // این هیچ‌وقت اجرا نمی‌شود در Win32+
}
```

---

## از دیدگاه Reverse Engineering و امنیتی

### 1. **یافتن Base Address برای Shellcode**

در exploit development، نیاز به `hInstance` واقعی برای دور زدن ASLR:

```c
// یافتن base address برنامه
HMODULE hBase = GetModuleHandle(NULL);

// یا از PEB (Process Environment Block)
__asm {
    mov eax, fs:[0x30]    ; PEB
    mov eax, [eax+0x08]   ; ImageBaseAddress
    mov hBase, eax
}
```

### 2. **Resource Extraction — استخراج malware**

بررسی منابع یک EXE مشکوک:

```c
HMODULE hMalware = LoadLibraryEx(L"suspect.exe", NULL, LOAD_LIBRARY_AS_DATAFILE);

// Enumerate resources
EnumResourceTypes(hMalware, EnumTypesFunc, 0);

// Extract embedded payload
HRSRC hRes = FindResource(hMalware, MAKEINTRESOURCE(666), RT_RCDATA);
HGLOBAL hData = LoadResource(hMalware, hRes);
void* pPayload = LockResource(hData);
DWORD size = SizeofResource(hMalware, hRes);

// حالا payload استخراج شده است
```

### 3. **Module Stomping — تکنیک injection**

```c
// بارگذاری یک DLL بی‌ضرر
HMODULE hDecoy = LoadLibrary(L"legitimate.dll");

// جایگزینی کد آن با shellcode
DWORD oldProtect;
VirtualProtect((LPVOID)hDecoy, shellcodeSize, PAGE_EXECUTE_READWRITE, &oldProtect);
memcpy((void*)hDecoy, shellcode, shellcodeSize);

// حالا hDecoy اشاره به کد مخرب دارد ولی به نظر legitimate است
```

### 4. **شناسایی DLL Injection**

بررسی moduleهای لود شده در یک process:

```c
HMODULE hMods[1024];
DWORD cbNeeded;
EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded);

for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
    WCHAR szModName[MAX_PATH];
    GetModuleFileNameEx(hProcess, hMods[i], szModName, MAX_PATH);
    
    // بررسی moduleهای مشکوک
    if (wcsstr(szModName, L"injected.dll")) {
        // احتمال injection!
        ReportThreat(hMods[i]);
    }
}
```

---

## HINSTANCE در .NET (WinForms/WPF)

در managed code، مفهوم `HINSTANCE` کمتر مستقیم است:

```csharp
// C# - WinForms
using System.Diagnostics;

// دریافت hInstance برنامه جاری
IntPtr hInstance = Process.GetCurrentProcess().MainModule.BaseAddress;

// یا از P/Invoke
[DllImport("kernel32.dll")]
static extern IntPtr GetModuleHandle(string lpModuleName);

IntPtr hInst = GetModuleHandle(null);
```

---

## جمع‌بندی

hInstance
= آدرس پایه module در حافظه

┌───────────────────────────────────────┐
│ استفاده‌ها:                            │
│ • ثبت Window Class                    │
│ • بارگذاری Resources (آیکون، منو، ...) │
│ • ساخت پنجره                          │
│ • تمایز بین moduleهای مختلف            │
└───────────────────────────────────────┘


**نکات کلیدی:**
- `hInstance` = base address فایل PE در حافظه
- `hInstance` != process ID (PID)
- هر process حتی از یک EXE، `hInstance` متفاوت دارد (ASLR)
- `hPrevInstance` همیشه NULL در Win32+
- برای دریافت: `GetModuleHandle(NULL)`
- از دید امنیتی: نقطه شروع برای resource extraction و module enumeration


### مثال 

![[Pasted image 20260626160136.png]]

همونطور که مشاهده میکنید این notepad ها مستقل هستند و هرکدوم modle و resouce های مربوط به خوشون رو دارند 
در واقع hInstance  مشخص کننده ادرس پایه اون فایل است 

پس به صورت خلاصه hInstance مشخص میکنه که ما با کدوم نمونه از برنامه مون داریم کار میکنیم
