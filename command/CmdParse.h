#ifndef CMDPARSE_H
#define CMDPARSE_H
#include <string>
#include <vector>

namespace yeguang{

class CmdParse{
public:
    typedef struct tagCommand{
        char flage_;
        bool param_;
        std::string help_;

        tagCommand(char flage, bool param, std::string help){
            flage_ = flage;
            param_ = param;
            help_  = help;
        }
    }TCommand;

    typedef std::vector<TCommand> TCmdList;

    CmdParse();
    virtual ~CmdParse();

    int Parse(int argc, char *argv[]);

protected:
    void SetCmdList(TCmdList& cmdlist);

    virtual int OnCmdFlagParser(char flag, std::string optarg = "") = 0;

    virtual int OnParserEnd() = 0;

	void Usage();

private:
	int GetOpt(int argc, char* argv[]);

    std::string commandlist_;
    std::string usages_;
};

}

#endif // CMDPARSE_H
