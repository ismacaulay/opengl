#pragma once

class IndexBuffer {
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline unsigned int getCount() { return count_; }

private:
    unsigned int rendererID_;
    unsigned int count_;
};
