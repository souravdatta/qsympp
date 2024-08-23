#ifndef __UTLITE_H__
#define __UTLITE_H__

#include <iostream>
#include <list>
#include <string>

namespace utlite
{

    struct UTException
    {
        unsigned int utex_line;
        std::string utex_what;
        std::string utex_file;
    };

    struct UTFailInfo
    {
        std::string utc_name;
        unsigned int utc_line;
        std::string utc_what;
        std::string utc_file;
    };

    class ResultStore
    {
    public:
        static ResultStore *getInstance()
        {
            static ResultStore *lsinstance = new ResultStore();
            return lsinstance;
        }

        void putFail(const UTFailInfo &l_info)
        {
            fail_lst.push_back(l_info);
        }

        void incPassCount()
        {
            this->pass_count++;
        }

        void show_results() const
        {
            std::cout << std::endl
                      << pass_count << " test case(s) passed." << std::endl;
            if (fail_lst.size() > 0)
            {
                for (std::list<UTFailInfo>::const_iterator lit = fail_lst.begin();
                     lit != fail_lst.end();
                     ++lit)
                {
                    std::cout << "Failed: " << lit->utc_name
                              << ", line: " << lit->utc_line
                              << ", file: " << lit->utc_file
                              << "\nexpr: " << lit->utc_what
                              << std::endl;
                }
            }
        }

    private:
        ResultStore() : pass_count(0)
        {
        }

        ResultStore(const ResultStore &)
        {
        }

        std::list<UTFailInfo> fail_lst;
        unsigned int pass_count;
    };

#define UT_ASSERT(COND)            \
    if (!(COND))                   \
    {                              \
        utlite::UTException lexp;  \
        lexp.utex_line = __LINE__; \
        lexp.utex_what = #COND;    \
        lexp.utex_file = __FILE__; \
        throw lexp;                \
    }

    template <class Suite>
    class UTCase
    {
    public:
        UTCase(const std::string &l_name, void (Suite::*l_func)(), Suite *l_inst) : utc_name(l_name), utc_func(l_func), utc_inst(l_inst)
        {
        }

        void do_test() const
        {
            std::cout << ".";
            try
            {
                (utc_inst->*utc_func)();
            }
            catch (const UTException &lexp)
            {
                UTFailInfo linfo;
                linfo.utc_name = this->utc_name;
                linfo.utc_line = lexp.utex_line;
                linfo.utc_what = lexp.utex_what;
                linfo.utc_file = lexp.utex_file;
                ResultStore::getInstance()->putFail(linfo);
                std::cout << "F";
                return;
            }
            catch (...)
            {
                // Unknown thing; bail out.
                throw;
            }
            ResultStore::getInstance()->incPassCount();
        }

    private:
        std::string utc_name;
        void (Suite::*utc_func)();
        Suite *utc_inst;
    };

    template <class ChildSuite>
    class Suite
    {
    public:
        virtual void addSuite() = 0;

        virtual void addTest(const std::string &l_name, void (ChildSuite::*l_func)(), ChildSuite *l_inst)
        {
            UTCase<ChildSuite> lcase(l_name, l_func, l_inst);
            suite.push_back(lcase);
        }

        virtual void run()
        {
            addSuite();
            typename std::list<UTCase<ChildSuite>>::iterator lit;
            for (lit = suite.begin(); lit != suite.end(); ++lit)
            {
                lit->do_test();
            }
            ResultStore::getInstance()->show_results();
        }

    protected:
        std::list<UTCase<ChildSuite>> suite;
    };

#define SUITE_BEGIN         \
    virtual void addSuite() \
    {

#define SUITE_END \
    }

#define UT(NAME, CLASS)                                 \
    do                                                  \
    {                                                   \
        addTest(#CLASS "::" #NAME, &CLASS::NAME, this); \
    } while (0)

} // namespace utlite

#endif
// end of utlite.h
