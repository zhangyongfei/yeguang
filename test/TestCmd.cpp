#include "CmdParse.h"
#include "Daemon.h"
#include "ConfigParse.h"
#include "ShareMemoryManager.h"
#include "Logger4.h"
#include "Log.h"
#include <stdio.h>
#include <unistd.h>

class MyCmdParse : public yeguang::CmdParse,
        public yeguang::Daemon{
public:
	MyCmdParse(){
		yeguang::CmdParse::TCmdList cmd_list;

        start = false;
        stop = false;

        cmd_list.push_back(TCommand('s', false, "启动进程"));
        cmd_list.push_back(TCommand('d', false, "打开调试模式"));
        cmd_list.push_back(TCommand('k', true,  "向进程发送信号"));
        cmd_list.push_back(TCommand('c', true,  "配置文件路径"));
        cmd_list.push_back(TCommand('u', false, "停止进程"));

        SetCmdList(cmd_list);

        SetChildCnt(1);
	}

	~MyCmdParse(){

	}

protected:
	int OnCmdFlagParser(char flag, std::string optarg){
        switch (flag)
        {
		case 's':
            start = true;
			break;
		case 'd':
			break;
		case 'k':
			break;
        case 'c':
            break;
		case 'u':
            stop = true;
			break;
		default:
			Usage();
			break;
        }

		return 0;
	}

    int OnParserEnd(){
        if(start && stop){
            Usage();
            return-1;
        }

        if(start){
            StartDaemon();
        } else if(stop){
            Stop();

            yeguang::ShareMemoryManager test;

            test.Init(0x1234, 0x1235, 100, 3);
            sleep(2);
            test.Exit();
        }
    }

	int Worker(){
        yeguang::Log::CreateLog(new yeguang::Logger4("./log4cplus.properties"));

        yeguang::ShareMemoryManager manager;

        manager.Init(0x1234, 0x1235, 100, 3);

        char buffer[255] = {0};

        while(1){
            int len = manager.FromMaster(buffer, sizeof(buffer));
            LogDebug("-----FromMaster: " << buffer);
            //printf("-----FromMaster:%s, len:%d\n", buffer, len);
            sleep(1);
            manager.ToMaster("Child Write", 11);
            sleep(1);
        }

        manager.Exit();
		return 0;
	}

    int Run(){
        yeguang::Log::CreateLog(new yeguang::Logger4("./log4cplus.properties"));

        yeguang::ShareMemoryManager manager;

        manager.Init(0x1234, 0x1235, 100, 3);

        char buffer[255] = {0};

        while(1){
            manager.ToChild("Master Write", 12);
            sleep(1);  
            int len = manager.FromChild(buffer, sizeof(buffer));            
            LogDebug("-------FromChild: " << buffer);
            //printf("-------FromChild:%s\n", buffer);          
            sleep(1);
        }

        manager.Exit();
        return 0;
    }

private:
    bool start;
    bool stop;
};

int main(int argc, char *argv[]){

    //yeguang::Log::CreateLog(new yeguang::Logger4("./log4cplus.properties"));
    //LogDebug("-------FromChild");

    MyCmdParse cmdparse;

    cmdparse.Parse(argc, argv);

    //yeguang::ConfigParse config;

    //config.Parse("./test.conf");

    //std::string value = "";
    //std::string line_num = "";

    //config.GetCofing("pid", value, line_num);

    //ConfgAssert(value != "", "pid isn't exist", line_num);

    //printf("%s\n", value.c_str());

	return 0;
}
