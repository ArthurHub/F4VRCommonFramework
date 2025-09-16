#include "ModBase.h"

#include <cpptrace/from_current.hpp>
#include <fmt/chrono.h>

#include "MainLoopHook.h"
#include "common/Logger.h"
#include "f4vr/VRControllersManager.h"
#include "ui/UIManager.h"

using namespace common;

namespace f4cf
{
    ModBase::ModBase(
        const std::string_view& name,
        const std::string_view& version,
        ConfigBase* config,
        const int trampolineAllocationSize):
        _name(name),
        _version(version),
        _config(config)
    {
        if (g_mod) {
            throw std::runtime_error("mod already initialized for " + g_mod->_name);
        }
        g_mod = this;

        // allocate enough space for patches and hooks
        F4SE::AllocTrampoline(trampolineAllocationSize);
    }

    /**
     * Run F4SE plugin query to check compatibility and fill out plugin info.
     */
    bool ModBase::onF4SEPluginQuery(const F4SE::QueryInterface* skse, F4SE::PluginInfo* info) const
    {
        bool success = false;
        CPPTRACE_TRY
            {
                logger::init(getLogFileName());
                logPluginGameStart();

                info->infoVersion = F4SE::PluginInfo::kVersion;
                info->name = _name.c_str();
                std::string tmp = _version;
                std::erase(tmp, '.');
                info->version = std::stoi(tmp);

                if (skse->IsEditor()) {
                    logger::critical("Loaded in editor, marking as incompatible");
                    // ReSharper disable once CppUnreachableCode
                } else if (skse->RuntimeVersion() < (REL::Module::IsF4() ? F4SE::RUNTIME_LATEST : F4SE::RUNTIME_LATEST_VR)) {
                    logger::critical("Unsupported runtime version {}", skse->RuntimeVersion().string());
                } else {
                    logger::info("F4SE Plugin Query v{} compatible: {} v{}", skse->F4SEVersion().string(), info->name, info->version);
                    success = true;
                }
            }
        CPPTRACE_CATCH(const std::exception& ex) {
            const auto stacktrace = cpptrace::from_current_exception().to_string();
            logger::error("Unhandled exception: {}\n{}", ex.what(), stacktrace);
        }
        return success;
    }

    /**
     * Run F4SE plugin load and initialize the plugin given the init handle.
     * Handle any exceptions and log them.
     */
    bool ModBase::onF4SEPluginLoad(const F4SE::LoadInterface* f4se)
    {
        bool success = false;
        CPPTRACE_TRY
            {
                logger::info("Init CommonLibF4 F4SE...");
                F4SE::Init(f4se, false);

                logger::info("Init config...");
                _config->load();

                logger::info("Register F4SE messages...");
                _messaging = F4SE::GetMessagingInterface();
                _messaging->RegisterListener(onF4VRSEMessage);

                logger::info("Load Mod...");
                onModLoaded(f4se);

                logger::info("Load successful");
                success = true;
            }
        CPPTRACE_CATCH(const std::exception& ex) {
            const auto stacktrace = cpptrace::from_current_exception().to_string();
            logger::error("Unhandled exception: {}\n{}", ex.what(), stacktrace);
        }
        return success;
    }

    /**
     * Runs on every game frame, main logic goes here.
     * Handle any exceptions and log them.
     */
    void ModBase::onFrameUpdateSafe()
    {
        CPPTRACE_TRY
            {
                f4vr::VRControllers.update(f4vr::isLeftHandedMode());

                onFrameUpdate();
            }
        CPPTRACE_CATCH(const std::exception& ex) {
            const auto stacktrace = cpptrace::from_current_exception().to_string();
            logger::critical("Error in onFrameUpdate: {}\n{}", ex.what(), stacktrace);
            throw;
        }
    }

    void ModBase::logPluginGameStart() const
    {
        const auto game = REL::Module::IsVR() ? "Fallout4VR" : "Fallout4";
        const auto runtimeVer = REL::Module::get().version();
        const auto dateTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        logger::info("Starting '{}' v{} ; {} v{} ; {:%Y-%m-%d %H:%M:%S%Ez} ; BaseAddress: 0x{:X}",
            _name, _version, game, runtimeVer.string(), fmt::localtime(dateTime), REL::Module::get().base());
    }

    /**
     * F4VRSE messages listener to handle game loaded, new game, and save loaded events.
     */
    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    void ModBase::onF4VRSEMessage(F4SE::MessagingInterface::Message* msg)
    {
        if (!msg) {
            return;
        }

        if (msg->type == F4SE::MessagingInterface::kGameLoaded) {
            // One time event fired after all plugins are loaded and game is full in main menu
            logger::info("F4VRSE On Game Loaded Message...");
            g_mod->onGameLoadedInner();
        }

        if (msg->type == F4SE::MessagingInterface::kPostLoadGame || msg->type == F4SE::MessagingInterface::kNewGame) {
            // If a game is loaded or a new game started re-initialize FRIK for clean slate
            logger::info("F4VRSE On Post Load Message...");
            g_mod->onGameSessionLoadedInner();
        }
    }

    /**
     * On game fully loaded initialize things that should be initialized only once.
     */
    void ModBase::onGameLoadedInner()
    {
        CPPTRACE_TRY
            {
                vrui::initUIManager();

                main_hook::hook();

                onGameLoaded();
            }
        CPPTRACE_CATCH(const std::exception& ex) {
            const auto stacktrace = cpptrace::from_current_exception().to_string();
            logger::critical("Error in onGameLoaded: {}\n{}", ex.what(), stacktrace);
            throw;
        }
    }

    /**
     * Game session can be initialized multiple times as it is fired on new game and save loaded events.
     * We should clear and reload as much of the game state as we can.
     */
    void ModBase::onGameSessionLoadedInner()
    {
        CPPTRACE_TRY
            {
                main_hook::validate();

                f4vr::VRControllers.reset();

                logger::info("Reload config...");
                _config->load();

                onGameSessionLoaded();
            }
        CPPTRACE_CATCH(const std::exception& ex) {
            const auto stacktrace = cpptrace::from_current_exception().to_string();
            logger::critical("Error in onGameSessionLoaded: {}\n{}", ex.what(), stacktrace);
            throw;
        }
    }
}
