#ifndef BRAINPARTY_PROJECTION_H
#define BRAINPARTY_PROJECTION_H

class Projection {
public:
    Projection();
    ~Projection();

    void setup(int width, int height);
    void map(float &x, float &y, bool with_offset=true);
    void unmap(float &x, float &y, bool with_offset=true);

private:
    int m_width;
    int m_height;
    float m_scale;
    float m_xoffset;
    float m_yoffset;
};

#endif /* BRAINPARTY_PROJECTION_H */
