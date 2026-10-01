/*
 * Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sys/wait.h>
#include <unistd.h>

#include "recovery_ui/device.h"
#include "recovery_ui/screen_ui.h"

class Exynos9810Device : public Device {
  public:
    explicit Exynos9810Device(RecoveryUI* ui) : Device(ui) {}

    bool PostWipeData() override {
        GetUI()->Print("-- Wiping FRP...\n");

        pid_t pid = fork();
        if (pid == 0) {
            execl("/system/bin/wipe-frp", "wipe-frp", nullptr);
            _exit(127);
        }

        int status;
        if (pid < 0 || waitpid(pid, &status, 0) != pid || !WIFEXITED(status) ||
            WEXITSTATUS(status) != 0) {
            GetUI()->Print("FRP wipe failed.\n");
            return false;
        }

        GetUI()->Print("FRP wipe complete.\n");
        return true;
    }
};

Device* make_device() {
    return new Exynos9810Device(new ScreenRecoveryUI);
}
