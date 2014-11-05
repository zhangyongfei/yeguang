#ifndef PROCESS_CTRL
#define PROCESS_CTRL
#include <string>

namespace yeguang{

class Daemon{
public:
    Daemon(std::string pid_path = "/var/tmp/current.pid", int child = 0);
	virtual ~Daemon();

    void SetPidPath(std::string pid_path) { pid_path_ = pid_path; };
    void SetChildCnt(int child) { child_ = child; };

    int StartSignle();
	int StartDaemon();

    int Stop();
protected:
    virtual int Worker() = 0;

    virtual int Run();

private:
    std::string pid_path_;
    int child_;
};
}


#endif
