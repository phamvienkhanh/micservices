#ifndef _CORE_UTILS_H_
#define _CORE_UTILS_H_

#include <uv.h>

#include <string>
#include <vector>

void allocBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
void freeWriteReqCb(uv_write_t* req, int status);
void freePipeCb(uv_handle_t* handle);

size_t randomId();

std::vector<std::string> strSplit(const std::string& str, char delimiter);
std::vector<std::string> strSplitFirts(const std::string& str, char delimiter);
std::string base32Decode(const std::string& input);
std::string toLower(const std::string& input);

uint64_t currentMs();

#endif