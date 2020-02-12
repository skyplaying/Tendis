// Copyright [2019] <eliotwang@tencent.com>
#ifndef SRC_TENDISPLUS_UTILS_TEST_UTIL_H_
#define SRC_TENDISPLUS_UTILS_TEST_UTIL_H_

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <thread>

#include "asio.hpp"
#include "tendisplus/server/server_entry.h"
#include "tendisplus/server/server_params.h"
#include "tendisplus/network/network.h"

// disable one test if needed
#define TEST_NO(a, b) void Test_no_##a##_##b()

namespace tendisplus {

using TestServer = std::shared_ptr<ServerEntry>;
using TestSession = std::shared_ptr<NetSession>;
using KeysWritten = std::set<std::string>;
using AllKeys = std::vector<KeysWritten>;

bool setupEnv();
void destroyEnv();
bool setupEnv(const std::string& v);
void destroyEnv(const std::string& v);
std::string getBulkValue(const std::string& reply, uint32_t index);
std::shared_ptr<ServerParams> makeServerParam(uint32_t port = 8811, uint32_t storeCnt = 0,
    const std::string& dir = "");
std::shared_ptr<ServerEntry> makeServerEntry(const std::shared_ptr<ServerParams>& cfg);
std::shared_ptr<NetSession> makeSession(std::shared_ptr<ServerEntry> server,
                                        std::shared_ptr<asio::io_context> ctx);

bool setupReplEnv();
void destroyReplEnv();

int genRand();
std::string randomStr(size_t s, bool maybeEmpty);

void testExpire1(std::shared_ptr<ServerEntry> svr);
void testExpire2(std::shared_ptr<ServerEntry> svr);
void testExpire(std::shared_ptr<ServerEntry> svr);
void testKV(std::shared_ptr<ServerEntry> svr);
void testMset(std::shared_ptr<ServerEntry> svr);
void testType(std::shared_ptr<ServerEntry> svr);
void testPf(std::shared_ptr<ServerEntry> svr);
void testZset(std::shared_ptr<ServerEntry> svr);
void testSet(std::shared_ptr<ServerEntry> svr);
void testZset3(std::shared_ptr<ServerEntry> svr);
void testZset4(std::shared_ptr<ServerEntry> svr);
void testZset2(std::shared_ptr<ServerEntry> svr);
void testHash1(std::shared_ptr<ServerEntry> svr);
void testHash2(std::shared_ptr<ServerEntry> svr);
void testList(std::shared_ptr<ServerEntry> svr);

void testAll(std::shared_ptr<ServerEntry> svr);

class WorkLoad {
 public:
    WorkLoad(TestServer server, TestSession session) :
        _session(session),
        _max_key_len(32) { }

    void init() { std::srand((uint32_t)msSinceEpoch()); }
    KeysWritten writeWork(RecordType, uint32_t count,
                          uint32_t maxlen = 0, bool sharename = true,
                          const char* key_suffix = NULL);
    void expireKeys(const AllKeys &keys, uint64_t ttl);
    void slaveof(const std::string& ip, uint32_t port);
    void flush();
    void delKeys(const KeysWritten &keys);
    void setMaxKeyLen(uint32_t max_key_len);
    Expected<uint64_t> getIntResult(const std::vector<std::string>& args);
    std::string getStringResult(const std::vector<std::string>& args);

 private:
    TestSession _session;
    uint32_t _max_key_len;
};
}  // namespace tendisplus

#endif  // SRC_TENDISPLUS_UTILS_TEST_UTIL_H_