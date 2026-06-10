#ifndef DISPLAYABLE_H
#define DISPLAYABLE_H

// Abstraction: common interface for entities displayed in reports.
class Displayable {
public:
    virtual void display() const = 0;
    virtual ~Displayable() = default;
};

#endif
