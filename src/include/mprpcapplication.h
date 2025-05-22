#pragma once

#include "mprpcconfig.h"

class MprpcApplication {

  public:
    static void Init(int argc, char** argv);
    static MprpcApplication& GetInstance();
  private:
    MprpcApplication() = default;
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;

    static MprpcConfig m_config;
};