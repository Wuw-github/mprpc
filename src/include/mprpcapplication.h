#pragma once

class MprpcApplication {

  public:
    static void Init(int argc, char** argv);
    static MprpcApplication& GetInstance();
  private:
    MyprpcApplication() = default;
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;

};