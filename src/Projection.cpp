#include "Projection.h"

#include "Config.h"

#include <algorithm>


static const int WORLD_WIDTH = 320;
static const int WORLD_HEIGHT = 480;


Projection::Projection()
    : m_width(0)
    , m_height(0)
    , m_scale(1.f)
    , m_xoffset(0.f)
    , m_yoffset(0.f)
{
}

Projection::~Projection()
{
}

void Projection::setup(int width, int height)
{
    m_width = width;
    m_height = height;

    float w = m_width, W = WORLD_WIDTH, wscale = w / W;
    float h = m_height, H = WORLD_HEIGHT, hscale = h / H;

    m_scale = std::min(wscale, hscale);
    m_xoffset = (w - W * m_scale) / 2.f;
    m_yoffset = (h - H * m_scale) / 2.f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrthof(0, w, h, 0, -100.0f, 100.0f);
    glTranslatef(m_xoffset, m_yoffset, 0.f);
    glScalef(m_scale, m_scale, 1.f);

    float x = 0.f, y = 0.f;
    unmap(x, y);
    unmap(W, H, false);
    y = m_height - H - y;

    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, W, H);
}


void Projection::map(float &x, float &y, bool with_offset)
{
    x = (x - (with_offset ? m_xoffset : 0.f)) / m_scale;
    y = (y - (with_offset ? m_yoffset : 0.f)) / m_scale;
}

void Projection::unmap(float &x, float &y, bool with_offset)
{
    x = (x * m_scale) + (with_offset ? m_xoffset : 0.f);
    y = (y * m_scale) + (with_offset ? m_yoffset : 0.f);
}
