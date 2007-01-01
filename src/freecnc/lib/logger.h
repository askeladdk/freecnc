#ifndef _LIB_LOGGER_H
#define _LIB_LOGGER_H

class VFile;

class Logger
{
public:
    Logger(const char *logname, int threshold);
    ~Logger();
    void gameMsg(const char *txt, ...);
    void renderGameMsg(bool r)
    {
        rendergamemsg = r;
    }

    void indent();
    void unindent();

private:
    VFile* logfile;
    int indentsteps, threshold;
    char indentString[64];
    bool rendergamemsg;
};

#endif

