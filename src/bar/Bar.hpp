#pragma once

#include <unordered_map>

#include "../defines.hpp"
#include "../ipc/ipc.hpp"
#include "BarCommands.hpp"
#include <chrono>

inline int barScreen = 0;

struct SDrawingContext {
    xcb_gcontext_t      GContext;
    xcb_font_t          Font;
};

enum ModuleAlignment {
    LEFT = 0,
    CENTER,
    RIGHT
};

struct SBarModule {
    ModuleAlignment alignment;
    std::string     value;
    std::string     icon;
    std::string     valueCalculated = "";
    uint64_t        color;
    uint64_t        bgcolor;

    uint64_t        updateEveryMs;
    std::chrono::system_clock::time_point updateLast;

    xcb_gcontext_t  bgcontext = NULL; // deprecated

    // PADS
    bool            isPad = false;
    int             pad = 0;
};

class CStatusBar {
public:

    EXPOSED_MEMBER(WindowID, xcb_window_t, i);
    EXPOSED_MEMBER(MonitorID, int, i);
    EXPOSED_MEMBER(StatusCommand, std::string, sz); // TODO: make the bar better
    EXPOSED_MEMBER(LastWindowName, std::string, sz);
    EXPOSED_MEMBER(LastWindowClass, std::string, sz);
    EXPOSED_MEMBER(IsCovered, bool, b);
    EXPOSED_MEMBER(HasTray, bool, b);

    void                draw();
    void                setup(int MonitorID);
    void                destroy();
    void                setupModule(SBarModule*);
    void                destroyModule(SBarModule*);
    void                ensureTrayClientDead(xcb_window_t);
    void                ensureTrayClientHidden(xcb_window_t, bool);
    void                setupTray();

    std::vector<int>    openWorkspaces;
    EXPOSED_MEMBER(CurrentWorkspace, int, i);

    std::vector<SBarModule> modules;

private:
    Vector2D            m_vecSize;
    Vector2D            m_vecPosition;

    xcb_pixmap_t        m_iPixmap;


    // Cairo

    cairo_surface_t*    m_pCairoSurface = nullptr;
    cairo_t*            m_pCairo        = nullptr;

    void                drawText(Vector2D, std::string, uint32_t, std::string);
    void                drawCairoRectangle(Vector2D, Vector2D, uint32_t);
    int                 getTextWidth(std::string, std::string);
    int                 drawModule(SBarModule*, int);
    int                 drawWorkspacesModule(SBarModule*, int);
    int                 getTextHalfY();

    std::unordered_map<std::string, SDrawingContext> m_mContexts;


    void                fixTrayOnCreate();
    void                saveTrayOnDestroy();
    int                 drawTrayModule(SBarModule*, int);
    xcb_window_t        trayWindowID = 0;
};

// Main thread for the bar. Is only initted once in main.cpp so we can do this.
int64_t barMainThread();