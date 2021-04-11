//https://raw.githubusercontent.com/chromium/chromium/master/extensions/common/feature_switch.cc

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "extensions/common/feature_switch.h"

#include "base/command_line.h"
#include "base/lazy_instance.h"
#include "base/strings/string_util.h"
#include "build/branding_buildflags.h"
#include "build/build_config.h"
#include "extensions/common/switches.h"

namespace extensions {

namespace {

// The switch load-media-router-component-extension is defined in
// chrome/common/chrome_switches.cc, but we can't depend on chrome here.
const char kLoadMediaRouterComponentExtensionFlag[] =
    "load-media-router-component-extension";

class CommonSwitches {
 public:
  CommonSwitches()
      : force_dev_mode_highlighting(switches::kForceDevModeHighlighting,
                                    FeatureSwitch::DEFAULT_DISABLED),
        // Intentionally no flag since turning this off outside of tests
        // is a security risk.
        prompt_for_external_extensions(nullptr,
#if defined(OS_WIN) || defined(OS_MAC)
                                       FeatureSwitch::DEFAULT_ENABLED),
#else
                                       FeatureSwitch::DEFAULT_DISABLED),
#endif
        embedded_extension_options(switches::kEmbeddedExtensionOptions,
                                   FeatureSwitch::DEFAULT_DISABLED),
        trace_app_source(switches::kTraceAppSource,
                         FeatureSwitch::DEFAULT_ENABLED),
        load_media_router_component_extension(
            kLoadMediaRouterComponentExtensionFlag,
#if BUILDFLAG(GOOGLE_CHROME_BRANDING)
            FeatureSwitch::DEFAULT_ENABLED)
#else
            FeatureSwitch::DEFAULT_DISABLED)
#endif  // BUILDFLAG(GOOGLE_CHROME_BRANDING)
  {
  }

  FeatureSwitch force_dev_mode_highlighting;

  // Should we prompt the user before allowing external extensions to install?
  // Default is yes.
  FeatureSwitch prompt_for_external_extensions;

  FeatureSwitch embedded_extension_options;
  FeatureSwitch trace_app_source;
  FeatureSwitch load_media_router_component_extension;
};

base::LazyInstance<CommonSwitches>::DestructorAtExit g_common_switches =
    LAZY_INSTANCE_INITIALIZER;

}  // namespace

FeatureSwitch* FeatureSwitch::force_dev_mode_highlighting() {
  return &g_common_switches.Get().force_dev_mode_highlighting;
}
FeatureSwitch* FeatureSwitch::prompt_for_external_extensions() {
  return &g_common_switches.Get().prompt_for_external_extensions;
}
FeatureSwitch* FeatureSwitch::embedded_extension_options() {
  return &g_common_switches.Get().embedded_extension_options;
}
FeatureSwitch* FeatureSwitch::trace_app_source() {
  return &g_common_switches.Get().trace_app_source;
}
FeatureSwitch* FeatureSwitch::load_media_router_component_extension() {
  return &g_common_switches.Get().load_media_router_component_extension;
}

FeatureSwitch::ScopedOverride::ScopedOverride(FeatureSwitch* feature,
                                              bool override_value)
    : feature_(feature),
      previous_value_(feature->GetOverrideValue()) {
  feature_->SetOverrideValue(
      override_value ? OVERRIDE_ENABLED : OVERRIDE_DISABLED);
}

FeatureSwitch::ScopedOverride::~ScopedOverride() {
  feature_->SetOverrideValue(previous_value_);
}

FeatureSwitch::FeatureSwitch(const char* switch_name,
                             DefaultValue default_value)
    : FeatureSwitch(base::CommandLine::ForCurrentProcess(),
                    switch_name,
                    default_value) {}

FeatureSwitch::FeatureSwitch(const base::CommandLine* command_line,
                             const char* switch_name,
                             DefaultValue default_value)
    : command_line_(command_line),
      switch_name_(switch_name),
      default_value_(default_value == DEFAULT_ENABLED),
      override_value_(OVERRIDE_NONE) {}

bool FeatureSwitch::IsEnabled() const {
  if (override_value_ != OVERRIDE_NONE)
    return override_value_ == OVERRIDE_ENABLED;
  if (!cached_value_.has_value())
    cached_value_ = ComputeValue();
  return cached_value_.value();
}

bool FeatureSwitch::ComputeValue() const {
  if (!switch_name_)
    return default_value_;

  std::string temp = command_line_->GetSwitchValueASCII(switch_name_);
  std::string switch_value;
  base::TrimWhitespaceASCII(temp, base::TRIM_ALL, &switch_value);

  if (switch_value == "1")
    return true;

  if (switch_value == "0")
    return false;

  if (command_line_->HasSwitch(GetLegacyEnableFlag()))
    return true;

  if (command_line_->HasSwitch(GetLegacyDisableFlag()))
    return false;

  return default_value_;
}

bool FeatureSwitch::HasValue() const {
  return override_value_ != OVERRIDE_NONE ||
         command_line_->HasSwitch(switch_name_) ||
         command_line_->HasSwitch(GetLegacyEnableFlag()) ||
         command_line_->HasSwitch(GetLegacyDisableFlag());
}

std::string FeatureSwitch::GetLegacyEnableFlag() const {
  DCHECK(switch_name_);
  return std::string("enable-") + switch_name_;
}

std::string FeatureSwitch::GetLegacyDisableFlag() const {
  DCHECK(switch_name_);
  return std::string("disable-") + switch_name_;
}

void FeatureSwitch::SetOverrideValue(OverrideValue override_value) {
  override_value_ = override_value;
}

FeatureSwitch::OverrideValue FeatureSwitch::GetOverrideValue() const {
  return override_value_;
}

}  // namespace extensions






//https://raw.githubusercontent.com/chromium/chromium/master/apps/switches.cc

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "apps/switches.h"

namespace apps {

// Loads an app from the specified directory and launches it.
const char kLoadAndLaunchApp[] = "load-and-launch-app";

}  // namespace apps




//https://raw.githubusercontent.com/chromium/chromium/master/ui/gfx/switches.cc
// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "build/build_config.h"
#include "ui/gfx/switches.h"

namespace switches {

// Scale factor to apply to every animation duration. Must be >= 0.0. This will
// only apply to LinearAnimation and its subclasses.
const char kAnimationDurationScale[] = "animation-duration-scale";

// Force disables font subpixel positioning. This affects the character glyph
// sharpness, kerning, hinting and layout.
const char kDisableFontSubpixelPositioning[] =
    "disable-font-subpixel-positioning";

// Enable native CPU-mappable GPU memory buffer support on Linux.
const char kEnableNativeGpuMemoryBuffers[] = "enable-native-gpu-memory-buffers";

// Forces whether the user desires reduced motion, regardless of system
// settings.
const char kForcePrefersReducedMotion[] = "force-prefers-reduced-motion";

// Run in headless mode, i.e., without a UI or display server dependencies.
const char kHeadless[] = "headless";

}  // namespace switches



//https://raw.githubusercontent.com/chromium/chromium/master/cc/base/switches.cc

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cc/base/switches.h"

#include "base/command_line.h"

namespace cc {
namespace switches {

const char kDisableThreadedAnimation[] = "disable-threaded-animation";

// Disables layer-edge anti-aliasing in the compositor.
const char kDisableCompositedAntialiasing[] =
    "disable-composited-antialiasing";

// Disables sending the next BeginMainFrame before the previous commit
// activates. Overrides the kEnableMainFrameBeforeActivation flag.
const char kDisableMainFrameBeforeActivation[] =
    "disable-main-frame-before-activation";

// Enables sending the next BeginMainFrame before the previous commit activates.
const char kEnableMainFrameBeforeActivation[] =
    "enable-main-frame-before-activation";

// Disabled defering all image decodes to the image decode service, ignoring
// DecodingMode preferences specified on PaintImage.
const char kDisableCheckerImaging[] = "disable-checker-imaging";

// Percentage of the browser controls need to be hidden before they will auto
// hide.
const char kBrowserControlsHideThreshold[] = "top-controls-hide-threshold";

// Percentage of the browser controls need to be shown before they will auto
// show.
const char kBrowserControlsShowThreshold[] = "top-controls-show-threshold";

// Re-rasters everything multiple times to simulate a much slower machine.
// Give a scale factor to cause raster to take that many times longer to
// complete, such as --slow-down-raster-scale-factor=25.
const char kSlowDownRasterScaleFactor[] = "slow-down-raster-scale-factor";

// Checks damage early and aborts the frame if no damage, so that clients like
// Android WebView don't invalidate unnecessarily.
const char kCheckDamageEarly[] = "check-damage-early";

// Enables the GPU benchmarking extension
const char kEnableGpuBenchmarking[] = "enable-gpu-benchmarking";

// Disables LayerTreeHost::OnMemoryPressure
const char kDisableLayerTreeHostMemoryPressure[] =
    "disable-layer-tree-host-memory-pressure";

// Renders a border around compositor layers to help debug and study
// layer compositing.
const char kShowCompositedLayerBorders[] = "show-composited-layer-borders";
const char kUIShowCompositedLayerBorders[] = "ui-show-composited-layer-borders";
// Parameters for kUIShowCompositedLayerBorders.
const char kCompositedRenderPassBorders[] = "renderpass";
const char kCompositedSurfaceBorders[] = "surface";
const char kCompositedLayerBorders[] = "layer";

#if DCHECK_IS_ON()
// Checks and logs double background blur as an error if any.
const char kLogOnUIDoubleBackgroundBlur[] = "log-on-ui-double-background-blur";
#endif

// Draws a heads-up-display showing Frames Per Second as well as GPU memory
// usage. If you also use --enable-logging=stderr --vmodule="head*=1" then FPS
// will also be output to the console log.
const char kShowFPSCounter[] = "show-fps-counter";
const char kUIShowFPSCounter[] = "ui-show-fps-counter";

// Renders a border that represents the bounding box for the layer's animation.
const char kShowLayerAnimationBounds[] = "show-layer-animation-bounds";
const char kUIShowLayerAnimationBounds[] = "ui-show-layer-animation-bounds";

// Show rects in the HUD around layers whose properties have changed.
const char kShowPropertyChangedRects[] = "show-property-changed-rects";
const char kUIShowPropertyChangedRects[] = "ui-show-property-changed-rects";

// Show rects in the HUD around damage as it is recorded into each render
// surface.
const char kShowSurfaceDamageRects[] = "show-surface-damage-rects";
const char kUIShowSurfaceDamageRects[] = "ui-show-surface-damage-rects";

// Show rects in the HUD around the screen-space transformed bounds of every
// layer.
const char kShowScreenSpaceRects[] = "show-screenspace-rects";
const char kUIShowScreenSpaceRects[] = "ui-show-screenspace-rects";

// Highlights layers that can't use lcd text. Layers containing no text won't
// be highlighted. See DebugColors::NonLCDTextHighlightColor() for the colors.
const char kHighlightNonLCDTextLayers[] = "highlight-non-lcd-text-layers";

// Switches the ui compositor to use layer lists instead of layer trees.
const char kUIEnableLayerLists[] = "ui-enable-layer-lists";

// Allows scaling clipped images in GpuImageDecodeCache. Note that this may
// cause color-bleeding.
// TODO(crbug.com/1157548): Remove this workaround flag once the underlying
// cache problems are solved.
const char kEnableClippedImageScaling[] = "enable-scaling-clipped-images";

// Prevents the layer tree unit tests from timing out.
const char kCCLayerTreeTestNoTimeout[] = "cc-layer-tree-test-no-timeout";

// Increases timeout for memory checkers.
const char kCCLayerTreeTestLongTimeout[] = "cc-layer-tree-test-long-timeout";

// Controls the duration of the scroll animation curve.
const char kCCScrollAnimationDurationForTesting[] =
    "cc-scroll-animation-duration-in-seconds";

}  // namespace switches
}  // namespace cc



//https://raw.githubusercontent.com/chromium/chromium/master/remoting/host/switches.cc

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "remoting/host/switches.h"

#include "build/build_config.h"

namespace remoting {

const char kElevateSwitchName[] = "elevate";
const char kHelpSwitchName[] = "help";
const char kProcessTypeSwitchName[] = "type";
const char kQuestionSwitchName[] = "?";
const char kVersionSwitchName[] = "version";

const char kProcessTypeController[] = "controller";
const char kProcessTypeDaemon[] = "daemon";
const char kProcessTypeDesktop[] = "desktop";
const char kProcessTypeHost[] = "host";
const char kProcessTypeRdpDesktopSession[] = "rdp_desktop_session";
const char kProcessTypeEvaluateCapability[] = "evaluate_capability";
const char kProcessTypeFileChooser[] = "file_chooser";
#if defined(OS_LINUX) || defined(OS_CHROMEOS)
const char kProcessTypeXSessionChooser[] = "xsession_chooser";
#endif  // defined(OS_LINUX) || defined(OS_CHROMEOS)

const char kEvaluateCapabilitySwitchName[] = "evaluate-type";

#if defined(OS_WIN)
const char kEvaluateD3D[] = "d3d-support";
const char kEvaluate3dDisplayMode[] = "3d-display-mode";
#endif

const char kParentWindowSwitchName[] = "parent-window";

const char kInputSwitchName[] = "input";
const char kOutputSwitchName[] = "output";

const char kMojoPipeToken[] = "mojo-pipe-token";

#if defined(OS_APPLE)
const char kCheckPermissionSwitchName[] = "check-permission";
const char kCheckAccessibilityPermissionSwitchName[] =
    "check-accessibility-permission";
const char kCheckScreenRecordingPermissionSwitchName[] =
    "check-screen-recording-permission";
const char kListAudioDevicesSwitchName[] = "list-audio-devices";
#endif  // defined OS_APPLE

}  // namespace remoting




//https://raw.githubusercontent.com/chromium/chromium/master/fuchsia/engine/switches.cc
// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fuchsia/engine/switches.h"

namespace switches {

const char kContentDirectories[] = "content-directories";
const char kDisableSoftwareVideoDecoders[] = "disable-software-video-decoders";
const char kEnableWidevine[] = "enable-widevine";
const char kIncognito[] = "incognito";
const char kPlayreadyKeySystem[] = "playready-key-system";
const char kRemoteDebuggerHandles[] = "remote-debugger-handles";
const char kUserAgentProductAndVersion[] = "user-agent-product";
const char kAllowRunningInsecureContent[] = "allow-running-insecure-content";
const char kUseLegacyMetricsService[] = "use-legacy-metrics-service";
const char kCorsExemptHeaders[] = "cors-exempt-headers";
const char kEnableCastStreamingReceiver[] = "enable-cast-streaming-receiver";
const char kCdmDataDirectory[] = "cdm-data-directory";
const char kCdmDataQuotaBytes[] = "cdm-data-quota-bytes";
const char kDataQuotaBytes[] = "data-quota-bytes";
const char kGoogleApiKey[] = "google-api-key";

}  // namespace switches



//https://raw.githubusercontent.com/chromium/chromium/master/sandbox/policy/switches.cc

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "sandbox/policy/switches.h"

#include "build/build_config.h"
#include "build/chromeos_buildflags.h"

#if defined(OS_WIN)
#include "base/command_line.h"
#include "base/win/windows_version.h"
#endif

namespace sandbox {
namespace policy {
namespace switches {

// Type of sandbox to apply to the process running the service, one of the
// values in the next block.
const char kServiceSandboxType[] = "service-sandbox-type";

// Must be in sync with "sandbox_type" values as used in service manager's
// manifest.json catalog files.
const char kNoneSandbox[] = "none";
const char kNoneSandboxAndElevatedPrivileges[] = "none_and_elevated";
const char kNetworkSandbox[] = "network";
const char kPpapiSandbox[] = "ppapi";
const char kUtilitySandbox[] = "utility";
const char kCdmSandbox[] = "cdm";
const char kPrintBackendSandbox[] = "print_backend";
const char kPrintCompositorSandbox[] = "print_compositor";
const char kAudioSandbox[] = "audio";
const char kSharingServiceSandbox[] = "sharing_service";
const char kSpeechRecognitionSandbox[] = "speech_recognition";
const char kVideoCaptureSandbox[] = "video_capture";

#if defined(OS_WIN)
const char kPdfConversionSandbox[] = "pdf_conversion";
const char kProxyResolverSandbox[] = "proxy_resolver";
const char kXrCompositingSandbox[] = "xr_compositing";
const char kIconReaderSandbox[] = "icon_reader";
const char kMediaFoundationCdmSandbox[] = "mf_cdm";
#endif  // OS_WIN

#if BUILDFLAG(IS_CHROMEOS_ASH)
const char kImeSandbox[] = "ime";
const char kTtsSandbox[] = "tts";
#endif  // BUILDFLAG(IS_CHROMEOS_ASH)

// Flags owned by the service manager sandbox.

// Enables the sandboxed processes to run without a job object assigned to them.
// This flag is required to allow Chrome to run in RemoteApps or Citrix. This
// flag can reduce the security of the sandboxed processes and allow them to do
// certain API calls like shut down Windows or access the clipboard. Also we
// lose the chance to kill some processes until the outer job that owns them
// finishes.
const char kAllowNoSandboxJob[] = "allow-no-sandbox-job";

// Allows debugging of sandboxed processes (see zygote_main_linux.cc).
const char kAllowSandboxDebugging[] = "allow-sandbox-debugging";

// Disables the GPU process sandbox.
const char kDisableGpuSandbox[] = "disable-gpu-sandbox";

// Disables usage of the namespace sandbox.
const char kDisableNamespaceSandbox[] = "disable-namespace-sandbox";

// Disable the seccomp filter sandbox (seccomp-bpf) (Linux only).
const char kDisableSeccompFilterSandbox[] = "disable-seccomp-filter-sandbox";

// Disable the setuid sandbox (Linux only).
const char kDisableSetuidSandbox[] = "disable-setuid-sandbox";

// Allows shmat() system call in the GPU sandbox.
const char kGpuSandboxAllowSysVShm[] = "gpu-sandbox-allow-sysv-shm";

// Makes GPU sandbox failures fatal.
const char kGpuSandboxFailuresFatal[] = "gpu-sandbox-failures-fatal";

// Disables the sandbox for all process types that are normally sandboxed.
// Meant to be used as a browser-level switch for testing purposes only.
const char kNoSandbox[] = "no-sandbox";

#if defined(OS_LINUX) || defined(OS_CHROMEOS)
// Instructs the zygote to launch without a sandbox. Processes forked from this
// type of zygote will apply their own custom sandboxes later.
const char kNoZygoteSandbox[] = "no-zygote-sandbox";
#endif

#if defined(OS_WIN)
// Allows third party modules to inject by disabling the BINARY_SIGNATURE
// mitigation policy on Win10+. Also has other effects in ELF.
const char kAllowThirdPartyModules[] = "allow-third-party-modules";

// Add additional capabilities to the AppContainer sandbox on the GPU process.
const char kAddGpuAppContainerCaps[] = "add-gpu-appcontainer-caps";

// Disables the sandbox and gives the process elevated privileges.
const char kNoSandboxAndElevatedPrivileges[] = "no-sandbox-and-elevated";

// Add additional capabilities to the AppContainer sandbox used for XR
// compositing.
const char kAddXrAppContainerCaps[] = "add-xr-appcontainer-caps";
#endif

#if defined(OS_MAC)
// Cause the OS X sandbox write to syslog every time an access to a resource
// is denied by the sandbox.
const char kEnableSandboxLogging[] = "enable-sandbox-logging";

// Disables Metal's shader cache, using the GPU sandbox to prevent access to it.
const char kDisableMetalShaderCache[] = "disable-metal-shader-cache";
#endif

// Flags spied upon from other layers.
const char kProcessType[] = "type";
const char kGpuProcess[] = "gpu-process";
const char kNaClBrokerProcess[] = "nacl-broker";
const char kNaClLoaderProcess[] = "nacl-loader";
const char kPpapiPluginProcess[] = "ppapi";
const char kRendererProcess[] = "renderer";
const char kUtilityProcess[] = "utility";
const char kCloudPrintServiceProcess[] = "service";
const char kZygoteProcessType[] = "zygote";
const char kRelauncherProcessType[] = "relauncher";

}  // namespace switches
}  // namespace policy
}  // namespace sandbox






//https://raw.githubusercontent.com/chromium/chromium/master/extensions/common/switches.cc

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "extensions/common/switches.h"

#include "build/chromeos_buildflags.h"

namespace extensions {

namespace switches {

// Allows non-https URL for background_page for hosted apps.
const char kAllowHTTPBackgroundPage[] = "allow-http-background-page";

// Allows the browser to load extensions that lack a modern manifest when that
// would otherwise be forbidden.
const char kAllowLegacyExtensionManifests[] =
    "allow-legacy-extension-manifests";

// Adds the given extension ID to all the permission allowlists.
const char kAllowlistedExtensionID[] = "whitelisted-extension-id";

// Enables extension options to be embedded in chrome://extensions rather than
// a new tab.
const char kEmbeddedExtensionOptions[] = "embedded-extension-options";

// Enable BLE Advertisiing in apps.
const char kEnableBLEAdvertising[] = "enable-ble-advertising-in-apps";

const char kDisableDesktopCaptureAudio[] =
    "disable-audio-support-for-desktop-share";

// Enables extension APIs that are in development.
const char kEnableExperimentalExtensionApis[] =
    "enable-experimental-extension-apis";

// Disable the net::URLRequestThrottlerManager functionality for
// requests originating from extensions.
const char kDisableExtensionsHttpThrottling[] =
    "disable-extensions-http-throttling";

// Marks a renderer as extension process.
const char kExtensionProcess[] = "extension-process";

// Enables extensions running scripts on chrome:// URLs.
// Extensions still need to explicitly request access to chrome:// URLs in the
// manifest.
const char kExtensionsOnChromeURLs[] = "extensions-on-chrome-urls";

// Whether to force developer mode extensions highlighting.
const char kForceDevModeHighlighting[] = "force-dev-mode-highlighting";

// Comma-separated list of paths to apps to load at startup. The first app in
// the list will be launched.
const char kLoadApps[] = "load-apps";

// Comma-separated list of paths to extensions to load at startup.
const char kLoadExtension[] = "load-extension";

#if BUILDFLAG(IS_CHROMEOS_ASH)
// Path to the unpacked test extension to load into the signin profile. The ID
// extension loaded must match kTestSigninProfileExtensionId.
const char kLoadSigninProfileTestExtension[] =
    "load-signin-profile-test-extension";

// Path to the unpacked test extension to load into guest mode. The extension ID
// must match kGuestModeTestExtensionId.
const char kLoadGuestModeTestExtension[] = "load-guest-mode-test-extension";
#endif

// Set the parameters for ExtensionURLLoaderThrottleBrowserTest.
const char kSetExtensionThrottleTestParams[] =
    "set-extension-throttle-test-params";

// Makes component extensions appear in chrome://settings/extensions.
const char kShowComponentExtensionOptions[] =
    "show-component-extension-options";

// Pass launch source to platform apps.
const char kTraceAppSource[] = "enable-trace-app-source";

// Enable package hash check: the .crx file sha256 hash sum should be equal to
// the one received from update manifest.
const char kEnableCrxHashCheck[] = "enable-crx-hash-check";

// Mute extension errors while working with new manifest version.
const char kAllowFutureManifestVersion[] = "allow-future-manifest-version";

}  // namespace switches

}  // namespace extensions





//https://raw.githubusercontent.com/chromium/chromium/master/components/ntp_tiles/switches.cc

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/ntp_tiles/switches.h"

namespace ntp_tiles {
namespace switches {

// Enables using the default search engine country to show country specific
// popular sites on the NTP.
const char kEnableNTPSearchEngineCountryDetection[] =
    "enable-ntp-search-engine-country-detection";

}  // namespace switches
}  // namespace ntp_tiles





//https://raw.githubusercontent.com/chromium/chromium/master/components/metal_util/switches.cc

// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/metal_util/switches.h"

namespace switches {

const char kDisableMetalTestShaders[] = "disable-metal-test-shaders";

}  // namespace switches





//https://raw.githubusercontent.com/chromium/chromium/master/components/viz/common/switches.cc

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/viz/common/switches.h"

#include "base/command_line.h"
#include "base/strings/string_number_conversions.h"
#include "build/chromeos_buildflags.h"
#include "components/viz/common/constants.h"

namespace switches {

// Screen width is useful for debugging. Shipping implementations should detect
// this.
const char kDeJellyScreenWidth[] = "de-jelly-screen-width";

// The default number of the BeginFrames to wait to activate a surface with
// dependencies.
const char kDeadlineToSynchronizeSurfaces[] =
    "deadline-to-synchronize-surfaces";

// Disables begin frame limiting in both cc scheduler and display scheduler.
// Also implies --disable-gpu-vsync (see //ui/gl/gl_switches.h).
const char kDisableFrameRateLimit[] = "disable-frame-rate-limit";

// Sets the number of max pending frames in the GL buffer queue to 1.
const char kDoubleBufferCompositing[] = "double-buffer-compositing";

const char kDraw1Point12Ms[] = "1-pt-12ms";
const char kDraw2Points6Ms[] = "2-pt-6ms";
const char kDraw1Point6Ms[] = "1-pt-6ms";
const char kDraw2Points3Ms[] = "2-pt-3ms";
const char kDrawPredictedInkPoint[] = "draw-predicted-ink-point";

// Experimental de-jelly support.
const char kEnableDeJelly[] = "enable-de-jelly";

// Enable compositing individual elements via hardware overlays when
// permitted by device.
// Setting the flag to "single-fullscreen" will try to promote a single
// fullscreen overlay and use it as main framebuffer where possible.
const char kEnableHardwareOverlays[] = "enable-hardware-overlays";

// Enables inspecting Viz Display Compositor objects. Default port is 9229.
// For local inspection use chrome://inspect#other
const char kEnableVizDevTools[] = "enable-viz-devtools";

// Enables hit-test debug logging.
const char kEnableVizHitTestDebug[] = "enable-viz-hit-test-debug";

#if defined(OS_CHROMEOS)
// ChromeOS uses one of two VideoDecoder implementations based on SoC/board
// specific configurations that are signalled via this command line flag.
// TODO(b/159825227): remove when the "old" video decoder is fully launched.
const char kPlatformDisallowsChromeOSDirectVideoDecoder[] =
    "platform-disallows-chromeos-direct-video-decoder";
#endif

// Effectively disables pipelining of compositor frame production stages by
// waiting for each stage to finish before completing a frame.
const char kRunAllCompositorStagesBeforeDraw[] =
    "run-all-compositor-stages-before-draw";

// Adds a DebugBorderDrawQuad to the top of the root RenderPass showing the
// damage rect after surface aggregation. Note that when enabled this feature
// sets the entire output rect as damaged after adding the quad to highlight the
// real damage rect, which could hide damage rect problems.
const char kShowAggregatedDamage[] = "show-aggregated-damage";

// Show debug borders for DC layers - red for overlays and blue for underlays.
// The debug borders are offset from the layer rect by a few pixels for clarity.
const char kShowDCLayerDebugBorders[] = "show-dc-layer-debug-borders";

base::Optional<uint32_t> GetDeadlineToSynchronizeSurfaces() {
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kRunAllCompositorStagesBeforeDraw)) {
    // In full-pipeline mode, surface deadlines should always be unlimited.
    return base::nullopt;
  }
  std::string deadline_to_synchronize_surfaces_string =
      command_line->GetSwitchValueASCII(
          switches::kDeadlineToSynchronizeSurfaces);
  if (deadline_to_synchronize_surfaces_string.empty())
    return viz::kDefaultActivationDeadlineInFrames;

  uint32_t activation_deadline_in_frames;
  if (!base::StringToUint(deadline_to_synchronize_surfaces_string,
                          &activation_deadline_in_frames)) {
    return base::nullopt;
  }
  return activation_deadline_in_frames;
}

}  // namespace switches





//https://raw.githubusercontent.com/chromium/chromium/master/chrome/browser/google/switches.cc

// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/google/switches.h"

namespace switches {

// Simulates a specific HRESULT error code returned by the update check.
// If the switch value is not specified (as hex) then it defaults to E_FAIL.
const char kSimulateUpdateHresult[] = "simulate-update-hresult";

// Simulates a GoogleUpdateErrorCode error by the update check.
// Must be supplied with |kSimulateUpdateHresult| switch.
const char kSimulateUpdateErrorCode[] = "simulate-update-error-code";

}  // namespace switches






//https://raw.githubusercontent.com/chromium/chromium/master/components/ui_devtools/switches.cc
// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/ui_devtools/switches.h"

namespace ui_devtools {
namespace switches {

// Enables DevTools server for UI (mus, ash, etc). Value should be the port the
// server is started on. Default port is 9223.
const char kEnableUiDevTools[] = "enable-ui-devtools";

}  // namespace switches
}  // namespace ui_devtools





//https://raw.githubusercontent.com/chromium/chromium/master/components/permissions/switches.cc
// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/permissions/switches.h"

namespace permissions {
namespace switches {

// Prevents permission prompts from appearing by denying instead of showing
// prompts.
const char kDenyPermissionPrompts[] = "deny-permission-prompts";

}  // namespace switches
}  // namespace permissions






//https://raw.githubusercontent.com/chromium/chromium/master/weblayer/public/common/switches.cc

// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "weblayer/public/common/switches.h"

namespace weblayer {
namespace switches {

// Makes WebLayer Shell use the given path for its data directory.
// NOTE: If changing this value, change the corresponding Java-side value in
// WebLayerBrowserTestsActivity.java#getUserDataDirectoryCommandLineSwitch() to
// match.
const char kWebLayerUserDataDir[] = "weblayer-user-data-dir";

}  // namespace switches
}  //  namespace weblayer






//https://raw.githubusercontent.com/chromium/chromium/master/components/query_tiles/switches.cc

// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/query_tiles/switches.h"

namespace query_tiles {
namespace features {
const base::Feature kQueryTilesGeoFilter{"QueryTilesGeoFilter",
                                         base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature kQueryTiles{"QueryTiles",
                                base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature kQueryTilesInNTP{"QueryTilesInNTP",
                                     base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature kQueryTilesInOmnibox{"QueryTilesInOmnibox",
                                         base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature kQueryTilesEnableQueryEditing{
    "QueryTilesEnableQueryEditing", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature kQueryTilesLocalOrdering{"QueryTilesLocalOrdering",
                                             base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature kQueryTilesRemoveTrendingTilesAfterInactivity{
    "QueryTilesRemoveTrendingAfterInactivity",
    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature kQueryTilesSegmentation{"QueryTilesSegmentation",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

bool IsEnabledQueryTilesInOmnibox() {
  return base::FeatureList::IsEnabled(features::kQueryTilesGeoFilter) &&
         base::FeatureList::IsEnabled(features::kQueryTilesInOmnibox);
}

}  // namespace features

namespace switches {
const char kQueryTilesSingleTier[] = "query-tiles-single-tier";

const char kQueryTilesCountryCode[] = "query-tiles-country-code";

const char kQueryTilesInstantBackgroundTask[] =
    "query-tiles-instant-background-task";

const char kQueryTilesEnableTrending[] = "query-tiles-enable-trending";

const char kQueryTilesMoreTrending[] = "query-tiles-more-trending";
}  // namespace switches
}  // namespace query_tiles





//https://raw.githubusercontent.com/chromium/chromium/master/extensions/shell/common/switches.cc

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "extensions/shell/common/switches.h"

#include "build/chromeos_buildflags.h"

namespace extensions {
namespace switches {

#if BUILDFLAG(IS_CHROMEOS_ASH)
// Allow roaming in the cellular network.
const char kAppShellAllowRoaming[] = "app-shell-allow-roaming";

// Size for the host window to create (i.e. "800x600").
const char kAppShellHostWindowSize[] = "app-shell-host-window-size";

// SSID of the preferred WiFi network.
const char kAppShellPreferredNetwork[] = "app-shell-preferred-network";
#endif

// TODO(crbug.com/1052397): Revisit the macro expression once build flag switch
// of lacros-chrome is complete.
#if defined(OS_LINUX) || BUILDFLAG(IS_CHROMEOS_LACROS)
// The directory breakpad should store minidumps in.
const char kCrashDumpsDir[] = "crash-dumps-dir";

// Enables metrics and crash reporting.
const char kEnableReporting[] = "enable-reporting";
#endif

}  // namespace switches
}  // namespace extensions



//https://raw.githubusercontent.com/chromium/chromium/master/components/ntp_snippets/switches.cc

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/ntp_snippets/switches.h"

namespace ntp_snippets {
namespace switches {

// If this flag is set, we will add downloaded snippets that are missing some
// critical data to the list.
const char kAddIncompleteSnippets[] = "ntp-snippets-add-incomplete";

}  // namespace switches
}  // namespace ntp_snippets





//https://raw.githubusercontent.com/chromium/chromium/master/third_party/blink/common/switches.cc
// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/public/common/switches.h"

namespace blink {
namespace switches {

// Allows processing of input before a frame has been committed.
// TODO(schenney): crbug.com/987626. Used by headless. Look for a way not
// involving a command line switch.
const char kAllowPreCommitInput[] = "allow-pre-commit-input";

// Set blink settings. Format is <name>[=<value],<name>[=<value>],...
// The names are declared in Settings.json5. For boolean type, use "true",
// "false", or omit '=<value>' part to set to true. For enum type, use the int
// value of the enum value. Applied after other command line flags and prefs.
const char kBlinkSettings[] = "blink-settings";

// Sets dark mode settings. Format is [<param>=<value>],[<param>=<value>],...
// The params take either int or float values. If params are not specified,
// the default dark mode settings is used. Valid params are given below.
// "InversionAlgorithm" takes int value of DarkModeInversionAlgorithm enum.
// "ImagePolicy" takes int value of DarkModeImagePolicy enum.
// "IsGrayScale" takes 1 or 0, 1 means grayscale is true, false otherwise.
// "TextBrightnessThreshold" takes 0 to 255 int value.
// "BackgroundBrightnessThreshold" takes 0 to 255 int value.
// "ContrastPercent" takes -1.0 to 1.0 float value. Higher the value, more
// the contrast.
// "ImageGrayScalePercent" takes 0.0 to 1.0 float value. Higher the value,
// image would be more grayish.
const char kDarkModeSettings[] = "dark-mode-settings";

// Sets the tile size used by composited layers.
const char kDefaultTileWidth[] = "default-tile-width";
const char kDefaultTileHeight[] = "default-tile-height";

// Disallow image animations to be reset to the beginning to avoid skipping
// many frames. Only effective if compositor image animations are enabled.
const char kDisableImageAnimationResync[] = "disable-image-animation-resync";

// When using CPU rasterizing disable low resolution tiling. This uses
// less power, particularly during animations, but more white may be seen
// during fast scrolling especially on slower devices.
const char kDisableLowResTiling[] = "disable-low-res-tiling";

// Disable partial raster in the renderer. Disabling this switch also disables
// the use of persistent gpu memory buffers.
const char kDisablePartialRaster[] = "disable-partial-raster";

// Disable the creation of compositing layers when it would prevent LCD text.
const char kDisablePreferCompositingToLCDText[] =
    "disable-prefer-compositing-to-lcd-text";

// Disables RGBA_4444 textures.
const char kDisableRGBA4444Textures[] = "disable-rgba-4444-textures";

// Disable multithreaded, compositor scrolling of web content.
const char kDisableThreadedScrolling[] = "disable-threaded-scrolling";

// Disable rasterizer that writes directly to GPU memory associated with tiles.
const char kDisableZeroCopy[] = "disable-zero-copy";

// Specify that all compositor resources should be backed by GPU memory buffers.
const char kEnableGpuMemoryBufferCompositorResources[] =
    "enable-gpu-memory-buffer-compositor-resources";

// When using CPU rasterizing generate low resolution tiling. Low res
// tiles may be displayed during fast scrolls especially on slower devices.
const char kEnableLowResTiling[] = "enable-low-res-tiling";

// Enable the creation of compositing layers when it would prevent LCD text.
const char kEnablePreferCompositingToLCDText[] =
    "enable-prefer-compositing-to-lcd-text";

// Enables RGBA_4444 textures.
const char kEnableRGBA4444Textures[] = "enable-rgba-4444-textures";

// Enables raster side dark mode for images.
const char kEnableRasterSideDarkModeForImages[] =
    "enable-raster-side-dark-mode-for-images";

// Enable rasterizer that writes directly to GPU memory associated with tiles.
const char kEnableZeroCopy[] = "enable-zero-copy";

// The number of multisample antialiasing samples for GPU rasterization.
// Requires MSAA support on GPU to have an effect. 0 disables MSAA.
const char kGpuRasterizationMSAASampleCount[] =
    "gpu-rasterization-msaa-sample-count";

// Used to communicate managed policy for the IntensiveWakeUpThrottling feature.
// This feature is typically controlled by base::Feature (see
// renderer/platform/scheduler/common/features.*) but requires an enterprise
// policy override. This is implicitly a tri-state, and can be either unset, or
// set to "1" for force enable, or "0" for force disable.
extern const char kIntensiveWakeUpThrottlingPolicy[] =
    "intensive-wake-up-throttling-policy";
extern const char kIntensiveWakeUpThrottlingPolicy_ForceDisable[] = "0";
extern const char kIntensiveWakeUpThrottlingPolicy_ForceEnable[] = "1";

// Sets the width and height above which a composited layer will get tiled.
const char kMaxUntiledLayerHeight[] = "max-untiled-layer-height";
const char kMaxUntiledLayerWidth[] = "max-untiled-layer-width";

// Sets the min tile height for GPU raster.
const char kMinHeightForGpuRasterTile[] = "min-height-for-gpu-raster-tile";

// Sets the timeout seconds of the network-quiet timers in IdlenessDetector.
// Used by embedders who want to change the timeout time in order to run web
// contents on various embedded devices and changeable network bandwidths in
// different regions. For example, it's useful when using FirstMeaningfulPaint
// signal to dismiss a splash screen.
const char kNetworkQuietTimeout[] = "network-quiet-timeout";

// Override the default value for the 'passive' field in javascript
// addEventListener calls. Values are defined as:
//  'documentonlytrue' to set the default be true only for document level nodes.
//  'true' to set the default to be true on all nodes (when not specified).
//  'forcealltrue' to force the value on all nodes.
const char kPassiveListenersDefault[] = "passive-listeners-default";

// Visibly render a border around layout shift rects in the web page to help
// debug and study layout shifts.
const char kShowLayoutShiftRegions[] = "show-layout-shift-regions";

// Visibly render a border around paint rects in the web page to help debug
// and study painting behavior.
const char kShowPaintRects[] = "show-paint-rects";

// Controls how text selection granularity changes when touch text selection
// handles are dragged. Should be "character" or "direction". If not specified,
// the platform default is used.
const char kTouchTextSelectionStrategy[] = "touch-selection-strategy";

// Used to communicate managed policy for the UserAgentClientHint feature.
// This feature is typically controlled by base::Feature (see
// renderer/platform/scheduler/common/features.*) but requires an enterprise
// policy override.

extern const char kUserAgentClientHintDisable[] =
    "user-agent-client-hint-disable";

}  // namespace switches
}  // namespace blink







//https://raw.githubusercontent.com/chromium/chromium/master/services/service_manager/switches.cc

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "services/service_manager/switches.h"

namespace service_manager {
namespace switches {

// Enable the tracing service.
const char kEnableTracing[] = "enable-service-manager-tracing";

}  // namespace switches
}  // namespace service_manager






//https://raw.githubusercontent.com/chromium/chromium/master/components/sync_sessions/switches.cc

// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/sync_sessions/switches.h"

namespace switches {

// Enables syncing Sessions data type in case when the window doesn't have open
// tabs anymore.
const base::Feature kSyncConsiderEmptyWindowsSyncable{
    "SyncConsiderEmptyWindowsSyncable", base::FEATURE_ENABLED_BY_DEFAULT};

}  // namespace switches




//https://raw.githubusercontent.com/chromium/chromium/master/components/sync_bookmarks/switches.cc

// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/sync_bookmarks/switches.h"
#include "base/feature_list.h"

namespace switches {

const base::Feature kSyncReuploadBookmarkFullTitles{
    "SyncReuploadBookmarkFullTitles", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature kSyncUseClientTagForBookmarkCommits{
    "SyncUseClientTagForBookmarkCommits", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature kSyncReuploadBookmarksUponMatchingData{
    "SyncReuploadBookmarksUponMatchingData", base::FEATURE_ENABLED_BY_DEFAULT};

}  // namespace switches




//https://raw.githubusercontent.com/chromium/chromium/master/components/webapps/common/switches.cc
// Copyright 2021 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/webapps/common/switches.h"

namespace webapps {
namespace switches {

// This flag causes the user engagement checks for showing app banners to be
// bypassed. It is intended to be used by developers who wish to test that their
// sites otherwise meet the criteria needed to show app banners.
const char kBypassAppBannerEngagementChecks[] =
    "bypass-app-banner-engagement-checks";

}  // namespace switches
}  // namespace webapps





//https://raw.githubusercontent.com/chromium/chromium/master/ash/wm/switchable_windows.cc

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/wm/switchable_windows.h"

#include <array>

#include "ash/public/cpp/shell_window_ids.h"
#include "ash/wm/desks/desks_util.h"
#include "base/containers/contains.h"
#include "ui/aura/window.h"

namespace ash {

namespace {

constexpr std::array<int, 2> kSwitchableContainers = {
    kShellWindowId_AlwaysOnTopContainer,
    kShellWindowId_PipContainer,
};

std::vector<int> GetSwitchableContainerIds() {
  std::vector<int> ids = desks_util::GetDesksContainersIds();
  for (const int id : kSwitchableContainers)
    ids.emplace_back(id);

  return ids;
}

}  // namespace

std::vector<aura::Window*> GetSwitchableContainersForRoot(
    aura::Window* root,
    bool active_desk_only) {
  DCHECK(root);
  DCHECK(root->IsRootWindow());

  std::vector<aura::Window*> containers;
  if (active_desk_only) {
    containers.push_back(desks_util::GetActiveDeskContainerForRoot(root));
    containers.push_back(
        root->GetChildById(kShellWindowId_AlwaysOnTopContainer));
    return containers;
  }

  for (const auto& id : GetSwitchableContainerIds()) {
    auto* container = root->GetChildById(id);
    DCHECK(container);
    containers.push_back(container);
  }

  return containers;
}

// TODO(afakhry): Rename this to a better name.
bool IsSwitchableContainer(const aura::Window* window) {
  if (!window)
    return false;

  return base::Contains(GetSwitchableContainerIds(), window->id());
}

}  // namespace ash






//https://raw.githubusercontent.com/chromium/chromium/master/components/cast_certificate/switches.cc

// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/cast_certificate/switches.h"

namespace cast_certificate {
namespace switches {

// When enabled by build flags, passing this argument allows the Cast
// authentication utils to use a custom root developer certificate in the trust
// store instead of the root Google-signed cert.
const char kCastDeveloperCertificatePath[] = "cast-developer-certificate-path";

}  // namespace switches
}  // namespace cast_certificate





//https://raw.githubusercontent.com/chromium/chromium/master/components/embedder_support/switches.cc

// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/embedder_support/switches.h"

namespace embedder_support {

// Disable auto-reload of error pages.
const char kDisableAutoReload[] = "disable-auto-reload";

// Disables pop-up blocking.
const char kDisablePopupBlocking[] = "disable-popup-blocking";

// Enable auto-reload of error pages.
const char kEnableAutoReload[] = "enable-auto-reload";

// Contains a list of feature names for which origin trial experiments should
// be disabled. Names should be separated by "|" characters.
const char kOriginTrialDisabledFeatures[] = "origin-trial-disabled-features";

// Contains a list of token signatures for which origin trial experiments should
// be disabled. Tokens should be separated by "|" characters.
const char kOriginTrialDisabledTokens[] = "origin-trial-disabled-tokens";

// Comma-separated list of keys which will override the default public keys for
// checking origin trial tokens.
const char kOriginTrialPublicKey[] = "origin-trial-public-key";

// Sets the Reporting API delay to under a second to allow much quicker reports.
const char kShortReportingDelay[] = "short-reporting-delay";

// A string used to override the default user agent with a custom one.
const char kUserAgent[] = "user-agent";

}  // namespace embedder_support






//https://raw.githubusercontent.com/chromium/chromium/master/ui/gl/gl_switches.cc

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_switches.h"

#include "base/stl_util.h"
#include "build/build_config.h"

#if defined(OS_ANDROID)
#include "base/android/build_info.h"
#endif

namespace gl {

const char kGLImplementationDesktopName[] = "desktop";
const char kGLImplementationAppleName[] = "apple";
const char kGLImplementationEGLName[] = "egl";
const char kGLImplementationANGLEName[] = "angle";
const char kGLImplementationSwiftShaderName[] = "swiftshader";
const char kGLImplementationSwiftShaderForWebGLName[] = "swiftshader-webgl";
const char kGLImplementationMockName[] = "mock";
const char kGLImplementationStubName[] = "stub";
const char kGLImplementationDisabledName[] = "disabled";

const char kANGLEImplementationDefaultName[]  = "default";
const char kANGLEImplementationD3D9Name[]     = "d3d9";
const char kANGLEImplementationD3D11Name[]    = "d3d11";
const char kANGLEImplementationD3D11on12Name[] = "d3d11on12";
const char kANGLEImplementationOpenGLName[]   = "gl";
const char kANGLEImplementationOpenGLEGLName[] = "gl-egl";
const char kANGLEImplementationOpenGLESName[] = "gles";
const char kANGLEImplementationOpenGLESEGLName[] = "gles-egl";
const char kANGLEImplementationNullName[] = "null";
const char kANGLEImplementationVulkanName[] = "vulkan";
const char kANGLEImplementationSwiftShaderName[] = "swiftshader";
const char kANGLEImplementationMetalName[] = "metal";
const char kANGLEImplementationNoneName[] = "";

// Special switches for "NULL"/stub driver implementations.
const char kANGLEImplementationD3D11NULLName[] = "d3d11-null";
const char kANGLEImplementationOpenGLNULLName[] = "gl-null";
const char kANGLEImplementationOpenGLESNULLName[] = "gles-null";
const char kANGLEImplementationVulkanNULLName[] = "vulkan-null";
const char kANGLEImplementationMetalNULLName[] = "metal-null";

// The command decoder names that can be passed to --use-cmd-decoder.
const char kCmdDecoderValidatingName[] = "validating";
const char kCmdDecoderPassthroughName[] = "passthrough";

// Swap chain formats for direct composition SDR video overlays.
const char kSwapChainFormatNV12[] = "nv12";
const char kSwapChainFormatYUY2[] = "yuy2";
const char kSwapChainFormatBGRA[] = "bgra";

}  // namespace gl

namespace switches {

// Disables use of D3D11.
const char kDisableD3D11[]                  = "disable-d3d11";

// Disables use of ES3 backend (use ES2 backend instead).
const char kDisableES3GLContext[]           = "disable-es3-gl-context";

// Disables use of ES3 backend at a lower level, for testing purposes.
// This isn't guaranteed to work everywhere, so it's test-only.
const char kDisableES3GLContextForTesting[] =
    "disable-es3-gl-context-for-testing";

// Disable workarounds for various GPU driver bugs.
const char kDisableGpuDriverBugWorkarounds[] =
    "disable-gpu-driver-bug-workarounds";

// Stop the GPU from synchronizing presentation with vblank.
const char kDisableGpuVsync[]               = "disable-gpu-vsync";

// Turns on GPU logging (debug build only).
const char kEnableGPUServiceLogging[]       = "enable-gpu-service-logging";

// Turns on calling TRACE for every GL call.
const char kEnableGPUServiceTracing[]       = "enable-gpu-service-tracing";

// Select which ANGLE backend to use. Options are:
//  default: Attempts several ANGLE renderers until one successfully
//           initializes, varying ES support by platform.
//  d3d9: Legacy D3D9 renderer, ES2 only.
//  d3d11: D3D11 renderer, ES2 and ES3.
//  warp: D3D11 renderer using software rasterization, ES2 and ES3.
//  gl: Desktop GL renderer, ES2 and ES3.
//  gles: GLES renderer, ES2 and ES3.
const char kUseANGLE[]                      = "use-angle";

// Use the Pass-through command decoder, skipping all validation and state
// tracking. Switch lives in ui/gl because it affects the GL binding
// initialization on platforms that would otherwise not default to using
// EGL bindings.
const char kUseCmdDecoder[] = "use-cmd-decoder";

// ANGLE features are defined per-backend in third_party/angle/include/platform
// Enables specified comma separated ANGLE features if found.
const char kEnableANGLEFeatures[] = "enable-angle-features";
// Disables specified comma separated ANGLE features if found.
const char kDisableANGLEFeatures[] = "disable-angle-features";

// Select which implementation of GL the GPU process should use. Options are:
//  desktop: whatever desktop OpenGL the user has installed (Linux and Mac
//           default).
//  egl: whatever EGL / GLES2 the user has installed (Windows default - actually
//       ANGLE).
//  swiftshader: The SwiftShader software renderer.
const char kUseGL[]                         = "use-gl";

// Inform Chrome that a GPU context will not be lost in power saving mode,
// screen saving mode, etc.  Note that this flag does not ensure that a GPU
// context will never be lost in any situations, say, a GPU reset.
const char kGpuNoContextLost[]              = "gpu-no-context-lost";

// Flag used for Linux tests: for desktop GL bindings, try to load this GL
// library first, but fall back to regular library if loading fails.
const char kTestGLLib[]                     = "test-gl-lib";

// Use hardware gpu, if available, for tests.
const char kUseGpuInTests[] = "use-gpu-in-tests";

// Enable use of the SGI_video_sync extension, which can have
// driver/sandbox/window manager compatibility issues.
const char kEnableSgiVideoSync[] = "enable-sgi-video-sync";

// Disables GL drawing operations which produce pixel output. With this
// the GL output will not be correct but tests will run faster.
const char kDisableGLDrawingForTests[] = "disable-gl-drawing-for-tests";

// Forces the use of software GL instead of hardware gpu.
const char kOverrideUseSoftwareGLForTests[] =
    "override-use-software-gl-for-tests";

// Disables specified comma separated GL Extensions if found.
const char kDisableGLExtensions[] = "disable-gl-extensions";

// Enables SwapBuffersWithBounds if it is supported.
const char kEnableSwapBuffersWithBounds[] = "enable-swap-buffers-with-bounds";

// Disables DirectComposition surface.
const char kDisableDirectComposition[] = "disable-direct-composition";

// Enables using DirectComposition video overlays, even if hardware overlays
// aren't supported.
const char kEnableDirectCompositionVideoOverlays[] =
    "enable-direct-composition-video-overlays";

// Disables using DirectComposition video overlays, even if hardware overlays
// are supported.
const char kDisableDirectCompositionVideoOverlays[] =
    "disable-direct-composition-video-overlays";

// Initialize the GPU process using the adapter with the specified LUID. This is
// only used on Windows, as LUID is a Windows specific structure.
const char kUseAdapterLuid[] = "use-adapter-luid";

// Enable kDirectCompositionForceFullDamage feature regardless of overlay
// support.
const char kDirectCompositionForceFullDamageForTesting[] =
    "direct-composition-force-full-damage-for-testing";

// Used for overriding the swap chain format for direct composition SDR video
// overlays.
const char kDirectCompositionVideoSwapChainFormat[] =
    "direct-composition-video-swap-chain-format";

// This is the list of switches passed from this file that are passed from the
// GpuProcessHost to the GPU Process. Add your switch to this list if you need
// to read it in the GPU process, else don't add it.
const char* const kGLSwitchesCopiedFromGpuProcessHost[] = {
    kDisableGpuDriverBugWorkarounds,
    kDisableGpuVsync,
    kDisableD3D11,
    kDisableES3GLContext,
    kDisableES3GLContextForTesting,
    kEnableGPUServiceLogging,
    kEnableGPUServiceTracing,
    kEnableSgiVideoSync,
    kGpuNoContextLost,
    kDisableGLDrawingForTests,
    kOverrideUseSoftwareGLForTests,
    kUseANGLE,
    kEnableSwapBuffersWithBounds,
    kDisableDirectComposition,
    kEnableDirectCompositionVideoOverlays,
    kDisableDirectCompositionVideoOverlays,
    kDirectCompositionForceFullDamageForTesting,
    kDirectCompositionVideoSwapChainFormat,
};
const int kGLSwitchesCopiedFromGpuProcessHostNumSwitches =
    base::size(kGLSwitchesCopiedFromGpuProcessHost);

}  // namespace switches

namespace features {

// Use BufferCount of 3 for the direct composition root swap chain.
const base::Feature kDCompTripleBufferRootSwapChain{
    "DCompTripleBufferRootSwapChain", base::FEATURE_DISABLED_BY_DEFAULT};

// Use BufferCount of 3 for direct composition video swap chains.
const base::Feature kDCompTripleBufferVideoSwapChain{
    "DCompTripleBufferVideoSwapChain", base::FEATURE_DISABLED_BY_DEFAULT};

// Forces Chrome's main backbuffer to full damage if the actual damage
// is large enough and allows DWM to consider the main backbuffer as an
// an overlay candidate.
const base::Feature kDirectCompositionForceFullDamage{
    "DirectCompositionForceFullDamage", base::FEATURE_ENABLED_BY_DEFAULT};

// Use presentation feedback event queries (must be enabled) to limit latency.
const base::Feature kDirectCompositionLowLatencyPresentation{
    "DirectCompositionLowLatencyPresentation",
    base::FEATURE_DISABLED_BY_DEFAULT};

// Allow overlay swapchain to present on all GPUs even if they only support
// software overlays.
const base::Feature kDirectCompositionSoftwareOverlays{
    "DirectCompositionSoftwareOverlays", base::FEATURE_DISABLED_BY_DEFAULT};

// Default to using ANGLE's OpenGL backend
const base::Feature kDefaultANGLEOpenGL{"DefaultANGLEOpenGL",
                                        base::FEATURE_DISABLED_BY_DEFAULT};

// Default to using ANGLE's Metal backend.
const base::Feature kDefaultANGLEMetal{"DefaultANGLEMetal",
                                       base::FEATURE_DISABLED_BY_DEFAULT};

// Default to using ANGLE's Vulkan backend.
const base::Feature kDefaultANGLEVulkan{"DefaultANGLEVulkan",
                                        base::FEATURE_DISABLED_BY_DEFAULT};

// Track current program's shaders at glUseProgram() call for crash report
// purpose. Only effective on Windows because the attached shaders may only
// be reliably retrieved with ANGLE backend.
const base::Feature kTrackCurrentShaders{"TrackCurrentShaders",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

bool IsDefaultANGLEVulkan() {
#if defined(OS_ANDROID)
  // No support for devices before Q -- exit before checking feature flags
  // so that devices are not counted in finch trials.
  if (base::android::BuildInfo::GetInstance()->sdk_int() <
      base::android::SDK_VERSION_Q)
    return false;
#endif  // defined(OS_ANDROID)
  return base::FeatureList::IsEnabled(kDefaultANGLEVulkan);
}

}  // namespace features






//https://raw.githubusercontent.com/chromium/chromium/master/chromeos/services/device_sync/switches.cc

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chromeos/services/device_sync/switches.h"

namespace chromeos {

namespace device_sync {

namespace switches {

// Overrides the default URL for Google APIs (https://www.googleapis.com) used
// by CryptAuth.
const char kCryptAuthHTTPHost[] = "cryptauth-http-host";

// Overrides the default URL for CryptAuth v2 Enrollment:
// https://cryptauthenrollment.googleapis.com.
const char kCryptAuthV2EnrollmentHTTPHost[] =
    "cryptauth-v2-enrollment-http-host";

// Overrides the default URL for CryptAuth v2 DeviceSync:
// https://cryptauthdevicesync.googleapis.com.
const char kCryptAuthV2DeviceSyncHTTPHost[] =
    "cryptauth-v2-devicesync-http-host";

}  // namespace switches

}  // namespace device_sync

}  // namespace chromeos





//https://raw.githubusercontent.com/chromium/chromium/master/base/base_switches.cc

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/base_switches.h"
#include "build/build_config.h"
#include "build/chromeos_buildflags.h"

namespace switches {

// Delays execution of TaskPriority::BEST_EFFORT tasks until shutdown.
const char kDisableBestEffortTasks[] = "disable-best-effort-tasks";

// Disables the crash reporting.
const char kDisableBreakpad[]               = "disable-breakpad";

// Comma-separated list of feature names to disable. See also kEnableFeatures.
const char kDisableFeatures[] = "disable-features";

// Force disabling of low-end device mode when set.
const char kDisableLowEndDeviceMode[] = "disable-low-end-device-mode";

// Indicates that crash reporting should be enabled. On platforms where helper
// processes cannot access to files needed to make this decision, this flag is
// generated internally.
const char kEnableCrashReporter[]           = "enable-crash-reporter";

// Comma-separated list of feature names to enable. See also kDisableFeatures.
const char kEnableFeatures[] = "enable-features";

// Force low-end device mode when set.
const char kEnableLowEndDeviceMode[]        = "enable-low-end-device-mode";

// Enable the use of background thread priorities for background tasks in the
// ThreadPool even on systems where it is disabled by default, e.g. due to
// concerns about priority inversions.
const char kEnableBackgroundThreadPool[] = "enable-background-thread-pool";

// This option can be used to force field trials when testing changes locally.
// The argument is a list of name and value pairs, separated by slashes. If a
// trial name is prefixed with an asterisk, that trial will start activated.
// For example, the following argument defines two trials, with the second one
// activated: "GoogleNow/Enable/*MaterialDesignNTP/Default/" This option can
// also be used by the browser process to send the list of trials to a
// non-browser process, using the same format. See
// FieldTrialList::CreateTrialsFromString() in field_trial.h for details.
const char kForceFieldTrials[]              = "force-fieldtrials";

// Generates full memory crash dump.
const char kFullMemoryCrashReport[] = "full-memory-crash-report";

// Logs information about all tasks posted with TaskPriority::BEST_EFFORT. Use
// this to diagnose issues that are thought to be caused by
// TaskPriority::BEST_EFFORT execution fences. Note: Tasks posted to a
// non-BEST_EFFORT UpdateableSequencedTaskRunner whose priority is later lowered
// to BEST_EFFORT are not logged.
const char kLogBestEffortTasks[] = "log-best-effort-tasks";

// Suppresses all error dialogs when present.
const char kNoErrorDialogs[]                = "noerrdialogs";

// Starts the sampling based profiler for the browser process at startup. This
// will only work if chrome has been built with the gn arg enable_profiling =
// true. The output will go to the value of kProfilingFile.
const char kProfilingAtStart[] = "profiling-at-start";

// Specifies a location for profiling output. This will only work if chrome has
// been built with the gyp variable profiling=1 or gn arg enable_profiling=true.
//
//   {pid} if present will be replaced by the pid of the process.
//   {count} if present will be incremented each time a profile is generated
//           for this process.
// The default is chrome-profile-{pid} for the browser and test-profile-{pid}
// for tests.
const char kProfilingFile[] = "profiling-file";

// Controls whether profile data is periodically flushed to a file. Normally
// the data gets written on exit but cases exist where chromium doesn't exit
// cleanly (especially when using single-process). A time in seconds can be
// specified.
const char kProfilingFlush[] = "profiling-flush";

// When running certain tests that spawn child processes, this switch indicates
// to the test framework that the current process is a child process.
const char kTestChildProcess[] = "test-child-process";

// When running certain tests that spawn child processes, this switch indicates
// to the test framework that the current process should not initialize ICU to
// avoid creating any scoped handles too early in startup.
const char kTestDoNotInitializeIcu[] = "test-do-not-initialize-icu";

// Sends trace events from these categories to a file.
// --trace-to-file on its own sends to default categories.
const char kTraceToFile[] = "trace-to-file";

// Specifies the file name for --trace-to-file. If unspecified, it will
// go to a default file name.
const char kTraceToFileName[] = "trace-to-file-name";

// Gives the default maximal active V-logging level; 0 is the default.
// Normally positive values are used for V-logging levels.
const char kV[] = "v";

// Gives the per-module maximal V-logging levels to override the value
// given by --v.  E.g. "my_module=2,foo*=3" would change the logging
// level for all code in source files "my_module.*" and "foo*.*"
// ("-inl" suffixes are also disregarded for this matching).
//
// Any pattern containing a forward or backward slash will be tested
// against the whole pathname and not just the module.  E.g.,
// "*/foo/bar/*=2" would change the logging level for all code in
// source files under a "foo/bar" directory.
const char kVModule[] = "vmodule";

// Will wait for 60 seconds for a debugger to come to attach to the process.
const char kWaitForDebugger[] = "wait-for-debugger";

#if defined(OS_WIN)
// Disable high-resolution timer on Windows.
const char kDisableHighResTimer[] = "disable-highres-timer";

// Disables the USB keyboard detection for blocking the OSK on Win8+.
const char kDisableUsbKeyboardDetect[]      = "disable-usb-keyboard-detect";
#endif

// TODO(crbug.com/1052397): Revisit the macro expression once build flag switch
// of lacros-chrome is complete.
#if defined(OS_LINUX) && !BUILDFLAG(IS_CHROMEOS_ASH) && \
    !BUILDFLAG(IS_CHROMEOS_LACROS)
// The /dev/shm partition is too small in certain VM environments, causing
// Chrome to fail or crash (see http://crbug.com/715363). Use this flag to
// work-around this issue (a temporary directory will always be used to create
// anonymous shared memory files).
const char kDisableDevShmUsage[] = "disable-dev-shm-usage";
#endif

#if defined(OS_POSIX)
// Used for turning on Breakpad crash reporting in a debug environment where
// crash reporting is typically compiled but disabled.
const char kEnableCrashReporterForTesting[] =
    "enable-crash-reporter-for-testing";
#endif

#if defined(OS_ANDROID)
// Enables the reached code profiler that samples all threads in all processes
// to determine which functions are almost never executed.
const char kEnableReachedCodeProfiler[] = "enable-reached-code-profiler";

// Specifies the profiling interval in microseconds for reached code profiler.
const char kReachedCodeSamplingIntervalUs[] =
    "reached-code-sampling-interval-us";

// Default country code to be used for search engine localization.
const char kDefaultCountryCodeAtInstall[] = "default-country-code";

// Adds additional thread idle time information into the trace event output.
const char kEnableIdleTracing[] = "enable-idle-tracing";

// The field trial parameters and their values when testing changes locally.
const char kForceFieldTrialParams[] = "force-fieldtrial-params";

#endif

#if defined(OS_LINUX) || defined(OS_CHROMEOS)
// Controls whether or not retired instruction counts are surfaced for threads
// in trace events on Linux.
//
// This flag requires the BPF sandbox to be disabled.
const char kEnableThreadInstructionCount[] = "enable-thread-instruction-count";

// TODO(crbug.com/1176772): Remove kEnableCrashpad and IsCrashpadEnabled() when
// Crashpad is fully enabled on Linux. Indicates that Crashpad should be
// enabled.
extern const char kEnableCrashpad[] = "enable-crashpad";
#endif

#if BUILDFLAG(IS_CHROMEOS_ASH) || BUILDFLAG(IS_CHROMEOS_LACROS)
// Override the default scheduling boosting value for urgent tasks.
// This can be adjusted if a specific chromeos device shows better perf/power
// ratio (e.g. by running video conference tests).
// Currently, this values directs to linux scheduler's utilization min clamp.
// Range is 0(no biased load) ~ 100(mamximum load value).
const char kSchedulerBoostUrgent[] = "scheduler-boost-urgent";
#endif

}  // namespace switches





//https://raw.githubusercontent.com/chromium/chromium/master/components/sync/invalidations/switches.cc

// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/sync/invalidations/switches.h"

namespace switches {

const base::Feature kSyncSendInterestedDataTypes = {
    "SyncSendInterestedDataTypes", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature kUseSyncInvalidations = {"UseSyncInvalidations",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature kUseSyncInvalidationsForWalletAndOffer = {
    "UseSyncInvalidationsForWalletAndOffer", base::FEATURE_DISABLED_BY_DEFAULT};

}  // namespace switches





//https://raw.githubusercontent.com/chromium/chromium/master/chrome/browser/video_tutorials/switches.cc

// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/video_tutorials/switches.h"

namespace video_tutorials {
namespace features {

const base::Feature kVideoTutorials{"VideoTutorials",
                                    base::FEATURE_DISABLED_BY_DEFAULT};

}  // namespace features

namespace switches {

const char kVideoTutorialsInstantFetch[] = "video-tutorials-instant-fetch";

}  // namespace switches

}  // namespace video_tutorials







//https://raw.githubusercontent.com/chromium/chromium/master/chrome/browser/lifetime/switch_utils.cc

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/lifetime/switch_utils.h"

#include <stddef.h>

#include "base/macros.h"
#include "build/build_config.h"
#include "chrome/common/chrome_switches.h"

#if defined(OS_WIN)
#include "base/strings/string_util.h"
#endif  // defined(OS_WIN)

namespace switches {

namespace {

// Switches enumerated here will be removed when a background instance of
// Chrome restarts itself. If your key is designed to only be used once,
// or if it does not make sense when restarting a background instance to
// pick up an automatic update, be sure to add it to this list.
constexpr const char* kSwitchesToRemoveOnAutorestart[] = {
    switches::kApp,
    switches::kAppId,
    switches::kForceFirstRun,
#if defined(OS_WIN)
    switches::kFromInstaller,
#endif
    switches::kGuest,
    switches::kIncognito,
    switches::kMakeDefaultBrowser,
    switches::kNoStartupWindow,
    switches::kRestoreLastSession,
    switches::kWinJumplistAction};

}  // namespace

void RemoveSwitchesForAutostart(base::CommandLine::SwitchMap* switch_list) {
  for (const char* switch_to_remove : kSwitchesToRemoveOnAutorestart)
    switch_list->erase(switch_to_remove);

#if defined(OS_WIN)
  // The relaunched browser process shouldn't reuse the /prefetch:# switch of
  // the current process because the process type can change (e.g. a process
  // initially launched in background can be relaunched in foreground).
  static const char kPrefetchSwitchPrefix[] = "prefetch:";
  auto it = switch_list->lower_bound(kPrefetchSwitchPrefix);
  if (it != switch_list->end() &&
      base::StartsWith(it->first, kPrefetchSwitchPrefix,
                       base::CompareCase::SENSITIVE)) {
    switch_list->erase(it);
  }
#endif  // defined(OS_WIN)
}

}  // namespace switches




//https://raw.githubusercontent.com/chromium/chromium/master/ui/gfx/x/x11_switches.cc

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/x/x11_switches.h"

namespace switches {

// Which X11 display to connect to. Emulates the GTK+ "--display=" command line
// argument.
const char kX11Display[] = "display";

// Disables MIT-SHM extension.
const char kNoXshm[] = "no-xshm";

}  // namespace switches




//https://raw.githubusercontent.com/chromium/chromium/master/components/search_provider_logos/switches.cc

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/search_provider_logos/switches.h"

namespace search_provider_logos {
namespace switches {

// Overrides the URL used to fetch the current Google Doodle.
// Example: https://www.google.com/async/ddljson
// Testing? Try:
//   https://www.gstatic.com/chrome/ntp/doodle_test/ddljson_android0.json
//   https://www.gstatic.com/chrome/ntp/doodle_test/ddljson_android1.json
//   https://www.gstatic.com/chrome/ntp/doodle_test/ddljson_android2.json
//   https://www.gstatic.com/chrome/ntp/doodle_test/ddljson_android3.json
//   https://www.gstatic.com/chrome/ntp/doodle_test/ddljson_android4.json
const char kGoogleDoodleUrl[] = "google-doodle-url";

// Use a static URL for the logo of the default search engine.
// Example: https://www.google.com/branding/logo.png
const char kSearchProviderLogoURL[] = "search-provider-logo-url";

// Overrides the Doodle URL to use for third-party search engines.
// Testing? Try:
//   https://www.gstatic.com/chrome/ntp/doodle_test/third_party_simple.json
//   https://www.gstatic.com/chrome/ntp/doodle_test/third_party_animated.json
const char kThirdPartyDoodleURL[] = "third-party-doodle-url";

}  // namespace switches
}  // namespace search_provider_logos




//https://raw.githubusercontent.com/chromium/chromium/master/ui/views/views_switches.cc

// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/views_switches.h"

#include "build/build_config.h"

namespace views {
namespace switches {

// Please keep alphabetized.

// Draws a semitransparent rect to indicate the bounds of each view.
const char kDrawViewBoundsRects[] = "draw-view-bounds-rects";

}  // namespace switches
}  // namespace views




//https://raw.githubusercontent.com/chromium/chromium/master/base/test/test_switches.cc

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/test/test_switches.h"

// Flag to show the help message.
const char switches::kHelpFlag[] = "help";

const char switches::kIsolatedScriptTestLauncherRetryLimit[] =
    "isolated-script-test-launcher-retry-limit";

// Makes pixel tests overwrite their reference if the output and reference
// don't match.
const char switches::kRebaselinePixelTests[] = "rebaseline-pixel-tests";

// Flag to run all tests and the launcher in a single process. Useful for
// debugging a specific test in a debugger.
const char switches::kSingleProcessTests[] = "single-process-tests";

// Maximum number of tests to run in a single batch.
const char switches::kTestLauncherBatchLimit[] = "test-launcher-batch-limit";

// Sets defaults desirable for the continuous integration bots, e.g. parallel
// test execution and test retries.
const char switches::kTestLauncherBotMode[] =
    "test-launcher-bot-mode";

// Makes it possible to debug the launcher itself. By default the launcher
// automatically switches to single process mode when it detects presence
// of debugger.
const char switches::kTestLauncherDebugLauncher[] =
    "test-launcher-debug-launcher";

// List of paths to files (separated by ';') containing test filters (one
// pattern per line).
const char switches::kTestLauncherFilterFile[] = "test-launcher-filter-file";

// Force running all requested tests and retries even if too many test errors
// occur.
const char switches::kTestLauncherForceRunBrokenTests[] =
    "test-launcher-force-run-broken-tests";

// Whether the test launcher should launch in "interactive mode", which disables
// timeouts (and may have other effects for specific test types).
const char switches::kTestLauncherInteractive[] = "test-launcher-interactive";

// Number of parallel test launcher jobs.
const char switches::kTestLauncherJobs[] = "test-launcher-jobs";

// Path to list of compiled in tests.
const char switches::kTestLauncherListTests[] = "test-launcher-list-tests";

// Path to test results file in our custom test launcher format.
const char switches::kTestLauncherOutput[] = "test-launcher-output";

// Causes the test launcher to print information about leaked files and/or
// directories in child process's temporary directories.
const char switches::kTestLauncherPrintTempLeaks[] =
    "test-launcher-print-temp-leaks";

// Flag controlling when test stdio is displayed as part of the launcher's
// standard output.
const char switches::kTestLauncherPrintTestStdio[] =
    "test-launcher-print-test-stdio";

// Print a writable path and exit (for internal use).
const char switches::kTestLauncherPrintWritablePath[] =
    "test-launcher-print-writable-path";

// Indicate how many retries are left. Tests in general should not pass in this
// flag. This flag is used for launcher to pass retries-left information
// to the runner process.
const char switches::kTestLauncherRetriesLeft[] = "test-launcher-retries-left";

// These two flags has the same effect, but don't use them at the same time.
// And isolated-script-test-launcher-retry-limit is preferred in the future.
// Maximum number of times to retry a test after failure.
const char switches::kTestLauncherRetryLimit[] = "test-launcher-retry-limit";

// Index of the test shard to run, starting from 0 (first shard) to total shards
// minus one (last shard).
const char switches::kTestLauncherShardIndex[] =
    "test-launcher-shard-index";

// Path to test results file with all the info from the test launcher.
const char switches::kTestLauncherSummaryOutput[] =
    "test-launcher-summary-output";

// Limit of test part results in the output. Default limit is 10.
// Negative value will completely disable limit.
const char switches::kTestLauncherTestPartResultsLimit[] =
    "test-launcher-test-part-results-limit";

// Total number of shards. Must be the same for all shards.
const char switches::kTestLauncherTotalShards[] =
    "test-launcher-total-shards";

// Time (in milliseconds) that the tests should wait before timing out.
const char switches::kTestLauncherTimeout[] = "test-launcher-timeout";

// Path where to save a trace of test launcher's execution.
const char switches::kTestLauncherTrace[] = "test-launcher-trace";

// TODO(phajdan.jr): Clean up the switch names.
const char switches::kTestTinyTimeout[] = "test-tiny-timeout";
const char switches::kUiTestActionMaxTimeout[] = "ui-test-action-max-timeout";
const char switches::kUiTestActionTimeout[] = "ui-test-action-timeout";

#if defined(OS_IOS)
// If enabled, runs unittests using the XCTest test runner.
const char switches::kEnableRunIOSUnittestsWithXCTest[] =
    "enable-run-ios-unittests-with-xctest";
// Write a compiled test json file to a location where writable.
const char switches::kWriteCompiledTestsJsonToWritablePath[] =
    "write-compiled-tests-json-to-writable-path";
#endif





//https://raw.githubusercontent.com/chromium/chromium/master/gpu/config/gpu_switches.cc

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gpu/config/gpu_switches.h"

namespace switches {

// Disable GPU rasterization, i.e. rasterize on the CPU only.
// Overrides the kEnableGpuRasterization flag.
const char kDisableGpuRasterization[] = "disable-gpu-rasterization";

// Disables mipmap generation in Skia. Used a workaround for select low memory
// devices, see https://crbug.com/1138979 for details.
const char kDisableMipmapGeneration[] = "disable-mipmap-generation";

// Allow heuristics to determine when a layer tile should be drawn with the
// Skia GPU backend. Only valid with GPU accelerated compositing.
const char kEnableGpuRasterization[] = "enable-gpu-rasterization";

// Select a different set of GPU blocklist entries with the specified
// test_group ID.
const char kGpuBlocklistTestGroup[] = "gpu-blocklist-test-group";

// Enable an extra set of GPU driver bug list entries with the specified
// test_group ID. Note the default test group (group 0) is still active.
const char kGpuDriverBugListTestGroup[] = "gpu-driver-bug-list-test-group";

// Passes encoded GpuPreferences to GPU process.
const char kGpuPreferences[] = "gpu-preferences";

// Ignores GPU blocklist.
const char kIgnoreGpuBlocklist[] = "ignore-gpu-blocklist";

// Allows explicitly specifying the shader disk cache size for embedded devices.
// Default value is 6MB. On Android, 2MB is default and 128KB for low-end
// devices.
const char kShaderDiskCacheSizeKB[] = "shader-disk-cache-size-kb";

// Disables the non-sandboxed GPU process for DX12 info collection
const char kDisableGpuProcessForDX12InfoCollection[] =
    "disable-gpu-process-for-dx12-info-collection";

const char kEnableUnsafeWebGPU[] = "enable-unsafe-webgpu";

// Enable validation layers in Dawn backends.
const char kEnableDawnBackendValidation[] = "enable-dawn-backend-validation";

// Set the Dawn features(toggles) enabled on the creation of Dawn devices.
const char kEnableDawnFeatures[] = "enable-dawn-features";

// Set the Dawn features(toggles) disabled on the creation of Dawn devices.
const char kDisableDawnFeatures[] = "disable-dawn-features";

// Increases the priority (to REALTIME_AUDIO) of gpu process and compositor
// thread.
// This is only to be used for perf tests on macOS for more reliable values.
const char kUseHighGPUThreadPriorityForPerfTests[] =
    "use-gpu-high-thread-priority-for-perf-tests";

// Start the non-sandboxed GPU process for DX12 and Vulkan info collection
// immediately after the browser starts. The default is to delay for 120
// seconds.
const char kNoDelayForDX12VulkanInfoCollection[] =
    "no-delay-for-dx12-vulkan-info-collection";

// Enables measures of how long GPU Main Thread was blocked between SwapBuffers
const char kEnableGpuBlockedTime[] = "enable-gpu-blocked-time";

// Passes the active graphics vendor id from browser process to info collection
// GPU process.
const char kGpuVendorId[] = "gpu-vendor-id";

// Passes the active graphics device id from browser process to info collection
// GPU process.
const char kGpuDeviceId[] = "gpu-device-id";

// Passes the active graphics sub system id from browser process to info
// collection GPU process.
const char kGpuSubSystemId[] = "gpu-sub-system-id";

// Passes the active graphics revision info from browser process to info
// collection GPU process.
const char kGpuRevision[] = "gpu-revision";

// Passes the active graphics driver version from browser process to info
// collection GPU process.
const char kGpuDriverVersion[] = "gpu-driver-version";

// Indicate that the this is being used by Android WebView and its draw functor
// is using vulkan.
const char kWebViewDrawFunctorUsesVulkan[] = "webview-draw-functor-uses-vulkan";

// Enables using protected memory for vulkan resources.
const char kEnableVulkanProtectedMemory[] = "enable-vulkan-protected-memory";

// Forces vulkan resources to use protected memory for vulkan compositing.
const char kEnforceVulkanProtectedMemory[] = "enforce-vulkan-protected-memory";

// Disables falling back to GL based hardware rendering if initializing Vulkan
// fails. This is to allow tests to catch regressions in Vulkan.
const char kDisableVulkanFallbackToGLForTesting[] =
    "disable-vulkan-fallback-to-gl-for-testing";

// Specifies the heap limit for Vulkan memory.
// TODO(crbug/1158000): Remove this switch.
const char kVulkanHeapMemoryLimitMb[] = "vulkan-heap-memory-limit-mb";

// Specifies the sync CPU limit for total Vulkan memory.
// TODO(crbug/1158000): Remove this switch.
const char kVulkanSyncCpuMemoryLimitMb[] = "vulkan-sync-cpu-memory-limit-mb";

// Crash Chrome if GPU process crashes. This is to force a test to fail when
// GPU process crashes unexpectedly.
const char kForceBrowserCrashOnGpuCrash[] = "force-browser-crash-on-gpu-crash";

}  // namespace switches



//https://raw.githubusercontent.com/chromium/chromium/master/ui/events/event_switches.cc

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "build/build_config.h"
#include "ui/events/event_switches.h"

namespace switches {

// Enable compensation for unstable pinch zoom. Some touch screens display
// significant amount of wobble when moving a finger in a straight line. This
// makes two finger scroll trigger an oscillating pinch zoom. See
// crbug.com/394380 for details.
const char kCompensateForUnstablePinchZoom[] =
    "compensate-for-unstable-pinch-zoom";

#if defined(OS_LINUX) || defined(OS_CHROMEOS)
// Tells chrome to interpret events from these devices as touch events. Only
// available with XInput 2 (i.e. X server 1.8 or above). The id's of the
// devices can be retrieved from 'xinput list'.
const char kTouchDevices[] = "touch-devices";

// Tells chrome to interpret events from these devices as pen events. Only
// available with XInput 2 (i.e. X server 1.8 or above). The id's of the
// devices can be retrieved from 'xinput list'.
const char kPenDevices[] = "pen-devices";
#endif

#if defined(USE_X11) || defined(USE_OZONE)
// Tells Chrome to do edge touch filtering. Useful for convertible tablet.
const char kEdgeTouchFiltering[] = "edge-touch-filtering";

// Disable CancelAllTouches() function for the implementation on cancel single
// touches.
const char kDisableCancelAllTouches[] = "disable-cancel-all-touches";
#endif

}  // namespace switches




//https://raw.githubusercontent.com/chromium/chromium/master/ui/base/ui_base_switches.cc
// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/ui_base_switches.h"

namespace switches {

#if defined(OS_MAC)
// Disable use of AVFoundation to draw video content.
const char kDisableAVFoundationOverlays[] = "disable-avfoundation-overlays";

// Fall back to using CAOpenGLLayers display content, instead of the IOSurface
// based overlay display path.
const char kDisableMacOverlays[] = "disable-mac-overlays";

// Disable animations for showing and hiding modal dialogs.
const char kDisableModalAnimations[] = "disable-modal-animations";

// Disable use of cross-process CALayers to display content directly from the
// GPU process on Mac.
const char kDisableRemoteCoreAnimation[] = "disable-remote-core-animation";

// Show borders around CALayers corresponding to overlays and partial damage.
const char kShowMacOverlayBorders[] = "show-mac-overlay-borders";
#endif

// Disables layer-edge anti-aliasing in the compositor.
const char kDisableCompositedAntialiasing[] = "disable-composited-antialiasing";

// Disables use of DWM composition for top level windows.
const char kDisableDwmComposition[] = "disable-dwm-composition";

// Disables touch event based drag and drop.
const char kDisableTouchDragDrop[] = "disable-touch-drag-drop";

// Disable re-use of non-exact resources to fulfill ResourcePool requests.
// Intended only for use in layout or pixel tests to reduce noise.
const char kDisallowNonExactResourceReuse[] =
    "disallow-non-exact-resource-reuse";

// Enables touch event based drag and drop.
const char kEnableTouchDragDrop[] = "enable-touch-drag-drop";

// Forces the caption style for WebVTT captions.
const char kForceCaptionStyle[] = "force-caption-style";

// Forces dark mode in UI for platforms that support it.
const char kForceDarkMode[] = "force-dark-mode";

// Forces high-contrast mode in native UI drawing, regardless of system
// settings. Note that this has limited effect on Windows: only Aura colors will
// be switched to high contrast, not other system colors.
const char kForceHighContrast[] = "force-high-contrast";

// The language file that we want to try to open. Of the form
// language[-country] where language is the 2 letter code from ISO-639.
const char kLang[] = "lang";

// Transform localized strings to be longer, with beginning and end markers to
// make truncation visually apparent.
const char kMangleLocalizedStrings[] = "mangle-localized-strings";

// Visualize overdraw by color-coding elements based on if they have other
// elements drawn underneath. This is good for showing where the UI might be
// doing more rendering work than necessary. The colors are hinting at the
// amount of overdraw on your screen for each pixel, as follows:
//
// True color: No overdraw.
// Blue: Overdrawn once.
// Green: Overdrawn twice.
// Pink: Overdrawn three times.
// Red: Overdrawn four or more times.
const char kShowOverdrawFeedback[] = "show-overdraw-feedback";

// Re-draw everything multiple times to simulate a much slower machine.
// Give a slow down factor to cause renderer to take that many times longer to
// complete, such as --slow-down-compositing-scale-factor=2.
const char kSlowDownCompositingScaleFactor[] =
    "slow-down-compositing-scale-factor";

// Tint composited color.
const char kTintCompositedContent[] = "tint-composited-content";

// Controls touch-optimized UI layout for top chrome.
const char kTopChromeTouchUi[] = "top-chrome-touch-ui";
const char kTopChromeTouchUiAuto[] = "auto";
const char kTopChromeTouchUiDisabled[] = "disabled";
const char kTopChromeTouchUiEnabled[] = "enabled";

// Disable partial swap which is needed for some OpenGL drivers / emulators.
const char kUIDisablePartialSwap[] = "ui-disable-partial-swap";

// Enables the ozone x11 clipboard for linux-chromeos.
const char kUseSystemClipboard[] = "use-system-clipboard";

}  // namespace switches




//https://raw.githubusercontent.com/chromium/chromium/master/media/midi/midi_switches.cc
// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "build/build_config.h"
#include "media/midi/midi_switches.h"

namespace midi {
namespace features {

#if defined(OS_WIN)
const base::Feature kMidiManagerWinrt{"MidiManagerWinrt",
                                      base::FEATURE_DISABLED_BY_DEFAULT};
#endif

}  // namespace features
}  // namespace midi




//https://raw.githubusercontent.com/chromium/chromium/master/gpu/config/gpu_switching.cc
// Copyright (c) 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gpu/config/gpu_switching.h"

#if defined(OS_MAC)
#include <OpenGL/OpenGL.h>
#endif

#include <algorithm>
#include <vector>

#include "base/command_line.h"
#include "gpu/config/gpu_driver_bug_workaround_type.h"
#include "gpu/config/gpu_info.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_switches.h"
#include "ui/gl/gpu_preference.h"

#if defined(OS_MAC)
#include "base/mac/mac_util.h"
#endif  // OS_MAC

namespace gpu {

namespace {

#if defined(OS_MAC)
typedef CGLPixelFormatObj PlatformPixelFormatObj;
#else
typedef void* PlatformPixelFormatObj;
#endif  // OS_MAC

PlatformPixelFormatObj g_discrete_pixel_format_obj = nullptr;

bool ContainsWorkaround(const std::vector<int32_t>& workarounds,
                        int32_t workaround) {
  return (std::find(workarounds.begin(), workarounds.end(), workaround) !=
          workarounds.end());
}

void ForceDiscreteGPU() {
  if (g_discrete_pixel_format_obj)
    return;
#if defined(OS_MAC)
  CGLPixelFormatAttribute attribs[1];
  attribs[0] = static_cast<CGLPixelFormatAttribute>(0);
  GLint num_pixel_formats = 0;
  CGLChoosePixelFormat(attribs, &g_discrete_pixel_format_obj,
                       &num_pixel_formats);
#endif  // OS_MAC
}

}  // namespace anonymous

bool SwitchableGPUsSupported(const GPUInfo& gpu_info,
                             const base::CommandLine& command_line) {
#if defined(OS_MAC)
  if (command_line.HasSwitch(switches::kUseGL) &&
      (command_line.GetSwitchValueASCII(switches::kUseGL) !=
           gl::kGLImplementationDesktopName &&
       command_line.GetSwitchValueASCII(switches::kUseGL) !=
           gl::kGLImplementationANGLEName)) {
    return false;
  }
  // Always allow offline renderers on ARM-based macs.
  // https://crbug.com/1131312
  switch (base::mac::GetCPUType()) {
    case base::mac::CPUType::kArm:
    case base::mac::CPUType::kTranslatedIntel:
      return true;
    default:
      break;
  }
  if (gpu_info.secondary_gpus.size() != 1) {
    return false;
  }

  // Only advertise that we have two GPUs to the rest of Chrome's code if we
  // find an Intel GPU and some other vendor's GPU. Otherwise we don't
  // understand the configuration and don't deal well with it (an example being
  // the dual AMD GPUs in recent Mac Pros). Motivation is explained in:
  // http://crbug.com/380026#c70.
  const uint32_t kVendorIntel = 0x8086;
  return ((gpu_info.gpu.vendor_id == kVendorIntel &&
           gpu_info.secondary_gpus[0].vendor_id != kVendorIntel) ||
          (gpu_info.gpu.vendor_id != kVendorIntel &&
           gpu_info.secondary_gpus[0].vendor_id == kVendorIntel));
#else
  return false;
#endif  // OS_MAC
}

void InitializeSwitchableGPUs(
    const std::vector<int32_t>& driver_bug_workarounds) {
  gl::GLContext::SetSwitchableGPUsSupported();
  if (ContainsWorkaround(driver_bug_workarounds, FORCE_HIGH_PERFORMANCE_GPU)) {
    gl::GLContext::SetForcedGpuPreference(gl::GpuPreference::kHighPerformance);
    ForceDiscreteGPU();
  } else if (ContainsWorkaround(driver_bug_workarounds, FORCE_LOW_POWER_GPU)) {
    gl::GLContext::SetForcedGpuPreference(gl::GpuPreference::kLowPower);
  }
}

void StopForceDiscreteGPU() {
#if defined(OS_MAC)
  if (g_discrete_pixel_format_obj) {
    CGLReleasePixelFormat(g_discrete_pixel_format_obj);
    g_discrete_pixel_format_obj = nullptr;
  }
#endif  // OS_MAC
}

}  // namespace gpu




//https://raw.githubusercontent.com/chromium/chromium/master/media/base/media_switches.cc
// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "media/base/media_switches.h"

#include "base/command_line.h"
#include "build/build_config.h"
#include "build/chromeos_buildflags.h"
#include "components/system_media_controls/linux/buildflags/buildflags.h"

namespace switches {

// Allow users to specify a custom buffer size for debugging purpose.
const char kAudioBufferSize[] = "audio-buffer-size";

// Set a timeout (in milliseconds) for the audio service to quit if there are no
// client connections to it. If the value is negative the service never quits.
const char kAudioServiceQuitTimeoutMs[] = "audio-service-quit-timeout-ms";

// Command line flag name to set the autoplay policy.
const char kAutoplayPolicy[] = "autoplay-policy";

const char kDisableAudioOutput[] = "disable-audio-output";

// Causes the AudioManager to fail creating audio streams. Used when testing
// various failure cases.
const char kFailAudioStreamCreation[] = "fail-audio-stream-creation";

// Set number of threads to use for video decoding.
const char kVideoThreads[] = "video-threads";

// Do not immediately suspend media in background tabs.
const char kDisableBackgroundMediaSuspend[] =
    "disable-background-media-suspend";

// Force to report VP9 as an unsupported MIME type.
const char kReportVp9AsAnUnsupportedMimeType[] =
    "report-vp9-as-an-unsupported-mime-type";

#if defined(OS_LINUX) || defined(OS_CHROMEOS) || defined(OS_FREEBSD) || \
    defined(OS_SOLARIS)
// The Alsa device to use when opening an audio input stream.
const char kAlsaInputDevice[] = "alsa-input-device";
// The Alsa device to use when opening an audio stream.
const char kAlsaOutputDevice[] = "alsa-output-device";
#endif  // defined(OS_LINUX) || defined(OS_CHROMEOS) || defined(OS_FREEBSD) ||
        // defined(OS_SOLARIS)

#if defined(OS_WIN)
// Use exclusive mode audio streaming for Windows Vista and higher.
// Leads to lower latencies for audio streams which uses the
// AudioParameters::AUDIO_PCM_LOW_LATENCY audio path.
// See http://msdn.microsoft.com/en-us/library/windows/desktop/dd370844.aspx
// for details.
const char kEnableExclusiveAudio[] = "enable-exclusive-audio";

// Use Windows WaveOut/In audio API even if Core Audio is supported.
const char kForceWaveAudio[] = "force-wave-audio";

// Instead of always using the hardware channel layout, check if a driver
// supports the source channel layout.  Avoids outputting empty channels and
// permits drivers to enable stereo to multichannel expansion.  Kept behind a
// flag since some drivers lie about supported layouts and hang when used.  See
// http://crbug.com/259165 for more details.
const char kTrySupportedChannelLayouts[] = "try-supported-channel-layouts";

// Number of buffers to use for WaveOut.
const char kWaveOutBuffers[] = "waveout-buffers";
#endif  // defined(OS_WIN)

#if defined(OS_FUCHSIA)
// Enables protected buffers for encrypted video streams.
const char kEnableProtectedVideoBuffers[] = "enable-protected-video-buffers";

// Forces protected memory for all output video buffers generated by
// FuchsiaVideoDecoder, including unencrypted streams. Ignored unless
// --enable-protected-video-buffers is also specified.
const char kForceProtectedVideoOutputBuffers[] =
    "force-protected-video-output-buffers";

const char kDisableAudioInput[] = "disable-audio-input";

// Present video content as overlays.
const char kUseOverlaysForVideo[] = "use-overlays-for-video";
#endif  // defined(OS_FUCHSIA)

#if defined(USE_CRAS)
// Use CRAS, the ChromeOS audio server.
const char kUseCras[] = "use-cras";
#endif  // defined(USE_CRAS)

// For automated testing of protected content, this switch allows specific
// domains (e.g. example.com) to skip asking the user for permission to share
// the protected media identifier. In this context, domain does not include the
// port number. User's content settings will not be affected by enabling this
// switch.
// Reference: http://crbug.com/718608
// Example:
// --unsafely-allow-protected-media-identifier-for-domain=a.com,b.ca
const char kUnsafelyAllowProtectedMediaIdentifierForDomain[] =
    "unsafely-allow-protected-media-identifier-for-domain";

// Use fake device for Media Stream to replace actual camera and microphone.
// For the list of allowed parameters, see
// FakeVideoCaptureDeviceFactory::ParseFakeDevicesConfigFromOptionsString().
const char kUseFakeDeviceForMediaStream[] = "use-fake-device-for-media-stream";

// Use an .y4m file to play as the webcam. See the comments in
// media/capture/video/file_video_capture_device.h for more details.
const char kUseFileForFakeVideoCapture[] = "use-file-for-fake-video-capture";

// Play a .wav file as the microphone. Note that for WebRTC calls we'll treat
// the bits as if they came from the microphone, which means you should disable
// audio processing (lest your audio file will play back distorted). The input
// file is converted to suit Chrome's audio buses if necessary, so most sane
// .wav files should work. You can pass either <path> to play the file looping
// or <path>%noloop to stop after playing the file to completion.
const char kUseFileForFakeAudioCapture[] = "use-file-for-fake-audio-capture";

// Use a fake device for accelerated decoding of MJPEG. This allows, for
// example, testing of the communication to the GPU service without requiring
// actual accelerator hardware to be present.
const char kUseFakeMjpegDecodeAccelerator[] =
    "use-fake-mjpeg-decode-accelerator";

// Disable hardware acceleration of mjpeg decode for captured frame, where
// available.
const char kDisableAcceleratedMjpegDecode[] =
    "disable-accelerated-mjpeg-decode";

// When running tests on a system without the required hardware or libraries,
// this flag will cause the tests to fail. Otherwise, they silently succeed.
const char kRequireAudioHardwareForTesting[] =
    "require-audio-hardware-for-testing";

// Mutes audio sent to the audio device so it is not audible during
// automated testing.
const char kMuteAudio[] = "mute-audio";

// Allows clients to override the threshold for when the media renderer will
// declare the underflow state for the video stream when audio is present.
// TODO(dalecurtis): Remove once experiments for http://crbug.com/470940 finish.
const char kVideoUnderflowThresholdMs[] = "video-underflow-threshold-ms";

// Disables the new rendering algorithm for webrtc, which is designed to improve
// the rendering smoothness.
const char kDisableRTCSmoothnessAlgorithm[] =
    "disable-rtc-smoothness-algorithm";

// Force media player using SurfaceView instead of SurfaceTexture on Android.
const char kForceVideoOverlays[] = "force-video-overlays";

// Allows explicitly specifying MSE audio/video buffer sizes as megabytes.
// Default values are 150M for video and 12M for audio.
const char kMSEAudioBufferSizeLimitMb[] = "mse-audio-buffer-size-limit-mb";
const char kMSEVideoBufferSizeLimitMb[] = "mse-video-buffer-size-limit-mb";

// Specifies the path to the Clear Key CDM for testing, which is necessary to
// support External Clear Key key system when library CDM is enabled. Note that
// External Clear Key key system support is also controlled by feature
// kExternalClearKeyForTesting.
const char kClearKeyCdmPathForTesting[] = "clear-key-cdm-path-for-testing";

// Overrides the default enabled library CDM interface version(s) with the one
// specified with this switch, which will be the only version enabled. For
// example, on a build where CDM 8, CDM 9 and CDM 10 are all supported
// (implemented), but only CDM 8 and CDM 9 are enabled by default:
//  --override-enabled-cdm-interface-version=8 : Only CDM 8 is enabled
//  --override-enabled-cdm-interface-version=9 : Only CDM 9 is enabled
//  --override-enabled-cdm-interface-version=10 : Only CDM 10 is enabled
//  --override-enabled-cdm-interface-version=11 : No CDM interface is enabled
// This can be used for local testing and debugging. It can also be used to
// enable an experimental CDM interface (which is always disabled by default)
// for testing while it's still in development.
const char kOverrideEnabledCdmInterfaceVersion[] =
    "override-enabled-cdm-interface-version";

// Overrides hardware secure codecs support for testing. If specified, real
// platform hardware secure codecs check will be skipped. Codecs are separated
// by comma. Valid codecs are "vp8", "vp9" and "avc1". For example:
//  --override-hardware-secure-codecs-for-testing=vp8,vp9
//  --override-hardware-secure-codecs-for-testing=avc1
// CENC encryption scheme is assumed to be supported for the specified codecs.
// If no valid codecs specified, no hardware secure codecs are supported. This
// can be used to disable hardware secure codecs support:
//  --override-hardware-secure-codecs-for-testing
const char kOverrideHardwareSecureCodecsForTesting[] =
    "override-hardware-secure-codecs-for-testing";

// Sets the default value for the kLiveCaptionEnabled preference to true.
const char kEnableLiveCaptionPrefForTesting[] =
    "enable-live-caption-pref-for-testing";

#if BUILDFLAG(ENABLE_PLATFORM_HEVC)
// Enables playback of clear (unencrypted) HEVC content for testing purposes.
const char kEnableClearHevcForTesting[] = "enable-clear-hevc-for-testing";
#endif

namespace autoplay {

// Autoplay policy that requires a document user activation.
const char kDocumentUserActivationRequiredPolicy[] =
    "document-user-activation-required";

// Autoplay policy that does not require any user gesture.
const char kNoUserGestureRequiredPolicy[] = "no-user-gesture-required";

// Autoplay policy to require a user gesture in order to play.
const char kUserGestureRequiredPolicy[] = "user-gesture-required";

}  // namespace autoplay

}  // namespace switches

namespace media {

// Prefer FFmpeg to LibVPX for Vp8 decoding with opaque alpha mode.
const base::Feature kFFmpegDecodeOpaqueVP8{"FFmpegDecodeOpaqueVP8",
                                           base::FEATURE_ENABLED_BY_DEFAULT};

// Only used for disabling overlay fullscreen (aka SurfaceView) in Clank.
const base::Feature kOverlayFullscreenVideo{"overlay-fullscreen-video",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

// TODO(crbug.com/1146594): Flip this to disabled in M92.
const base::Feature kEnableMediaInternals{"enable-media-internals",
                                          base::FEATURE_ENABLED_BY_DEFAULT};

// Enable Picture-in-Picture.
const base::Feature kPictureInPicture {
  "PictureInPicture",
#if defined(OS_ANDROID)
      base::FEATURE_DISABLED_BY_DEFAULT
#else
      base::FEATURE_ENABLED_BY_DEFAULT
#endif
};

// Only decode preload=metadata elements upon visibility.
// TODO(crbug.com/879406): Remove this after M76 ships to stable
const base::Feature kPreloadMetadataLazyLoad{"PreloadMetadataLazyLoad",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

// Let videos be resumed via remote controls (for example, the notification)
// when in background.
const base::Feature kResumeBackgroundVideo {
  "resume-background-video",
#if defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// When enabled, MediaCapabilities will check with GPU Video Accelerator
// Factories to determine isPowerEfficient = true/false.
const base::Feature kMediaCapabilitiesQueryGpuFactories{
    "MediaCapabilitiesQueryGpuFactories", base::FEATURE_ENABLED_BY_DEFAULT};

// Enable Media Capabilities with finch-parameters.
const base::Feature kMediaCapabilitiesWithParameters{
    "MediaCapabilitiesWithParameters", base::FEATURE_ENABLED_BY_DEFAULT};

// Display the Cast overlay button on the media controls.
const base::Feature kMediaCastOverlayButton{"MediaCastOverlayButton",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

// Use AndroidOverlay for more cases than just player-element fullscreen?  This
// requires that |kOverlayFullscreenVideo| is true, else it is ignored.
const base::Feature kUseAndroidOverlayAggressively{
    "UseAndroidOverlayAggressively", base::FEATURE_ENABLED_BY_DEFAULT};

// If enabled, RTCVideoDecoderAdapter will wrap a DecoderStream as a video
// decoder, rather than using MojoVideoDecoder.  This causes the RTC external
// decoder to have all the decoder selection / fallback/forward logic of the
// non-RTC pipeline.
// TODO(liberato): This also causes the external decoder to use software
// decoding sometimes, which changes the interpretation of "ExternalDecoder".
const base::Feature kUseDecoderStreamForWebRTC{
    "UseDecoderStreamForWebRTC", base::FEATURE_DISABLED_BY_DEFAULT};

// If enabled, when RTCVideoDecoderAdapter is used then SW decoders will be
// exposed directly to WebRTC.
const base::Feature kExposeSwDecodersToWebRTC{
    "ExposeSwDecodersToWebRTC", base::FEATURE_DISABLED_BY_DEFAULT};

// Let video without audio be paused when it is playing in the background.
const base::Feature kBackgroundVideoPauseOptimization{
    "BackgroundVideoPauseOptimization", base::FEATURE_ENABLED_BY_DEFAULT};

// CDM host verification is enabled by default. Can be disabled for testing.
// Has no effect if ENABLE_CDM_HOST_VERIFICATION buildflag is false.
const base::Feature kCdmHostVerification{"CdmHostVerification",
                                         base::FEATURE_ENABLED_BY_DEFAULT};

// Use per-CDM-type, per-user and per-site CDM processes (for library CDM). If
// disabled, the CDM processes are only per-CDM-type, meaning different sites
// using the same CDM type would share one CDM process.
const base::Feature kCdmProcessSiteIsolation{"CdmProcessSiteIsolation",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

// Make MSE garbage collection algorithm more aggressive when we are under
// moderate or critical memory pressure. This will relieve memory pressure by
// releasing stale data from MSE buffers.
const base::Feature kMemoryPressureBasedSourceBufferGC{
    "MemoryPressureBasedSourceBufferGC", base::FEATURE_DISABLED_BY_DEFAULT};

// Approach original pre-REC MSE object URL autorevoking behavior, though await
// actual attempt to use the object URL for attachment to perform revocation.
// This will hopefully reduce runtime memory bloat for pages that do not
// explicitly detach their HTMLME+MSE object collections nor explicitly revoke
// the object URLs used to attach HTMLME+MSE. When disabled, revocation only
// occurs when application explicitly revokes the object URL, or upon the
// execution context teardown for the MediaSource object. When enabled,
// revocation occurs upon successful start of attachment of HTMLME to the object
// URL. Note, rather than immediately scheduling a task to revoke upon the URL's
// creation, as at least one other browser does and the original File API
// pattern used to follow, this delay until attachment start enables new
// scenarios that could use the object URL for attaching HTMLME+MSE cross-thread
// (MSE-in-workers), where there could be significant delay between the worker
// thread creation of the object URL and the main thread usage of the object URL
// for starting attachment to HTMLME.
const base::Feature kRevokeMediaSourceObjectURLOnAttach{
    "RevokeMediaSourceObjectURLOnAttach", base::FEATURE_ENABLED_BY_DEFAULT};

// Enable saving playback information in a crash trace, to see if some codecs
// are crashier than others.
const base::Feature kD3D11PrintCodecOnCrash{"D3D11PrintCodecOnCrash",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

// Enable The D3D11 Video decoder.
const base::Feature kD3D11VideoDecoder{"D3D11VideoDecoder",
                                       base::FEATURE_DISABLED_BY_DEFAULT};

// Tell D3D11VideoDecoder to ignore workarounds for zero copy.  Requires that
// kD3D11VideoDecoder is enabled.
const base::Feature kD3D11VideoDecoderIgnoreWorkarounds{
    "D3D11VideoDecoderIgnoreWorkarounds", base::FEATURE_DISABLED_BY_DEFAULT};

// Enable D3D11VideoDecoder to decode VP9 profile 2 (10 bit) video.
const base::Feature kD3D11VideoDecoderVP9Profile2{
    "D3D11VideoDecoderEnableVP9Profile2", base::FEATURE_DISABLED_BY_DEFAULT};

// Enable D3D11VideoDecoder to decode AV1 video.
const base::Feature kD3D11VideoDecoderAV1{"D3D11VideoDecoderEnableAV1",
                                          base::FEATURE_DISABLED_BY_DEFAULT};

// Tell D3D11VideoDecoder not to switch the D3D11 device to multi-threaded mode.
// This is to help us track down IGD crashes.
const base::Feature kD3D11VideoDecoderSkipMultithreaded{
    "D3D11VideoDecoderSkipMultithreaded", base::FEATURE_DISABLED_BY_DEFAULT};

// If enabled, D3D11VideoDecoder will always copy instead of bind textures.
const base::Feature kD3D11VideoDecoderAlwaysCopy{
    "D3D11VideoDecoderAlwaysCopy", base::FEATURE_DISABLED_BY_DEFAULT};

// If enabled, D3D11VideoDecoder may (but is not required to) choose to mark
// VideoFrames as overlayable.
const base::Feature kD3D11VideoDecoderAllowOverlay{
    "D3D11VideoDecoderAllowOverlay", base::FEATURE_ENABLED_BY_DEFAULT};

// If enabled, D3D11VideoDecoder will enable HDR support even if the OS doesn't.
const base::Feature kD3D11VideoDecoderForceEnableHDR{
    "D3D11VideoDecoderForceEnableHDR", base::FEATURE_DISABLED_BY_DEFAULT};

// Falls back to other decoders after audio/video decode error happens. The
// implementation may choose different strategies on when to fallback. See
// DecoderStream for details. When disabled, playback will fail immediately
// after a decode error happens. This can be useful in debugging and testing
// because the behavior is simpler and more predictable.
const base::Feature kFallbackAfterDecodeError{"FallbackAfterDecodeError",
                                              base::FEATURE_ENABLED_BY_DEFAULT};

// Use Gav1VideoDecoder to decode AV1 streams.
const base::Feature kGav1VideoDecoder{"Gav1VideoDecoder",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

// Show toolbar button that opens dialog for controlling media sessions.
const base::Feature kGlobalMediaControls {
  "GlobalMediaControls",
#if defined(OS_WIN) || defined(OS_MAC) || defined(OS_LINUX) || \
    BUILDFLAG(IS_CHROMEOS_LACROS)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// Auto-dismiss global media controls.
const base::Feature kGlobalMediaControlsAutoDismiss{
    "GlobalMediaControlsAutoDismiss", base::FEATURE_ENABLED_BY_DEFAULT};

// Show Cast sessions in Global Media Controls. It is no-op if
// kGlobalMediaControls is not enabled.
const base::Feature kGlobalMediaControlsForCast{
    "GlobalMediaControlsForCast", base::FEATURE_ENABLED_BY_DEFAULT};

// Allow Global Media Controls in system tray of CrOS.
const base::Feature kGlobalMediaControlsForChromeOS{
    "GlobalMediaControlsForChromeOS", base::FEATURE_ENABLED_BY_DEFAULT};

constexpr base::FeatureParam<kCrosGlobalMediaControlsPinOptions>::Option
    kCrosGlobalMediaControlsParamOptions[] = {
        {kCrosGlobalMediaControlsPinOptions::kPin, "default-pinned"},
        {kCrosGlobalMediaControlsPinOptions::kNotPin, "default-unpinned"},
        {kCrosGlobalMediaControlsPinOptions::kHeuristic, "heuristic"}};

constexpr base::FeatureParam<kCrosGlobalMediaControlsPinOptions>
    kCrosGlobalMediaControlsPinParam(
        &kGlobalMediaControlsForChromeOS,
        "CrosGlobalMediaControlsPinParam",
        kCrosGlobalMediaControlsPinOptions::kHeuristic,
        &kCrosGlobalMediaControlsParamOptions);

// Allow global media controls notifications to be dragged out into overlay
// notifications. It is no-op if kGlobalMediaControls is not enabled.
const base::Feature kGlobalMediaControlsOverlayControls{
    "GlobalMediaControlsOverlayControls", base::FEATURE_DISABLED_BY_DEFAULT};

// Show picture-in-picture button in Global Media Controls.
const base::Feature kGlobalMediaControlsPictureInPicture {
  "GlobalMediaControlsPictureInPicture",
#if defined(OS_WIN) || defined(OS_MAC) || defined(OS_LINUX) || \
    BUILDFLAG(IS_CHROMEOS_LACROS)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// Enable selection of audio output device in Global Media Controls.
const base::Feature kGlobalMediaControlsSeamlessTransfer{
    "GlobalMediaControlsSeamlessTransfer", base::FEATURE_DISABLED_BY_DEFAULT};

// Enable an updated version of the Global Media Controls UI.
const base::Feature kGlobalMediaControlsModernUI{
    "GlobalMediaControlsModernUI", base::FEATURE_DISABLED_BY_DEFAULT};

// CanPlayThrough issued according to standard.
const base::Feature kSpecCompliantCanPlayThrough{
    "SpecCompliantCanPlayThrough", base::FEATURE_ENABLED_BY_DEFAULT};

// Controls usage of SurfaceLayer for MediaStreams.
const base::Feature kSurfaceLayerForMediaStreams{
    "SurfaceLayerForMediaStreams", base::FEATURE_ENABLED_BY_DEFAULT};

// Disables the real audio output stream after silent audio has been delivered
// for too long. Should save quite a bit of power in the muted video case.
const base::Feature kSuspendMutedAudio{"SuspendMutedAudio",
                                       base::FEATURE_ENABLED_BY_DEFAULT};

// Enables using the media history store to store media engagement metrics.
const base::Feature kUseMediaHistoryStore{"UseMediaHistoryStore",
                                          base::FEATURE_ENABLED_BY_DEFAULT};

// Use R16 texture for 9-16 bit channel instead of half-float conversion by CPU.
const base::Feature kUseR16Texture{"use-r16-texture",
                                   base::FEATURE_DISABLED_BY_DEFAULT};

// Enables the Unified Autoplay policy by overriding the platform's default
// autoplay policy.
const base::Feature kUnifiedAutoplay{"UnifiedAutoplay",
                                     base::FEATURE_ENABLED_BY_DEFAULT};

#if defined(OS_LINUX)
// Enable vaapi video decoding on linux. This is already enabled by default on
// chromeos, but needs an experiment on linux.
const base::Feature kVaapiVideoDecodeLinux{"VaapiVideoDecoder",
                                           base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature kVaapiVideoEncodeLinux{"VaapiVideoEncoder",
                                           base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_LINUX)

// Enable VA-API hardware decode acceleration for AV1.
const base::Feature kVaapiAV1Decoder{"VaapiAV1Decoder",
                                     base::FEATURE_ENABLED_BY_DEFAULT};

// Enable VA-API hardware low power encoder for all codecs on intel Gen9x gpu.
const base::Feature kVaapiLowPowerEncoderGen9x{
    "VaapiLowPowerEncoderGen9x", base::FEATURE_DISABLED_BY_DEFAULT};

// Reject creation of encode/decode VAContexts when the requested resolution is
// outside the enumerated minimum and maximum. TODO(b/171041334): Remove and
// enable by default once the ARC++ hw codecs issue is fixed.
const base::Feature kVaapiEnforceVideoMinMaxResolution{
    "VaapiEnforceVideoMinMaxResolution", base::FEATURE_DISABLED_BY_DEFAULT};

// Ensure the advertised minimum supported resolution is larger than or equal to
// a given one (likely QVGA + 1) for certain codecs/modes and platforms, for
// performance reasons. This does not affect JPEG decoding.
const base::Feature kVaapiVideoMinResolutionForPerformance{
    "VaapiVideoMinResolutionForPerformance", base::FEATURE_ENABLED_BY_DEFAULT};

// Enable VA-API hardware encode acceleration for VP8.
const base::Feature kVaapiVP8Encoder{"VaapiVP8Encoder",
                                     base::FEATURE_ENABLED_BY_DEFAULT};

// Enable VA-API hardware encode acceleration for VP9.
const base::Feature kVaapiVP9Encoder{"VaapiVP9Encoder",
                                     base::FEATURE_ENABLED_BY_DEFAULT};

#if defined(ARCH_CPU_X86_FAMILY) && BUILDFLAG(IS_CHROMEOS_ASH)
// Enable VP9 k-SVC decoding with HW decoder for webrtc use case on ChromeOS.
const base::Feature kVp9kSVCHWDecoding{"Vp9kSVCHWDecoding",
                                       base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(ARCH_CPU_X86_FAMILY) && BUILDFLAG(IS_CHROMEOS_ASH)

// Inform video blitter of video color space.
const base::Feature kVideoBlitColorAccuracy{"video-blit-color-accuracy",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

// Enables support for External Clear Key (ECK) key system for testing on
// supported platforms. On platforms that do not support ECK, this feature has
// no effect.
const base::Feature kExternalClearKeyForTesting{
    "ExternalClearKeyForTesting", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables the Live Caption feature.
const base::Feature kLiveCaption{"LiveCaption",
                                 base::FEATURE_ENABLED_BY_DEFAULT};

// Use the Speech On-Device API (SODA) to power the Live Caption feature instead
// of the Cloud-based Open Speech API.
const base::Feature kUseSodaForLiveCaption{"UseSodaForLiveCaption",
                                           base::FEATURE_ENABLED_BY_DEFAULT};

// Live Caption can be used in multiple languages, as opposed to just English.
const base::Feature kLiveCaptionMultiLanguage{
    "LiveCaptionMultiLanguage", base::FEATURE_DISABLED_BY_DEFAULT};

// Live Caption runs system-wide on ChromeOS, as opposed to just in the browser.
const base::Feature kLiveCaptionSystemWideOnChromeOS{
    "LiveCaptionSystemWideOnChromeOS", base::FEATURE_DISABLED_BY_DEFAULT};

// Prevents UrlProvisionFetcher from making a provisioning request. If
// specified, any provisioning request made will not be sent to the provisioning
// server, and the response will indicate a failure to communicate with the
// provisioning server.
const base::Feature kFailUrlProvisionFetcherForTesting{
    "FailUrlProvisionFetcherForTesting", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables hardware secure decryption if supported by hardware and CDM.
// TODO(xhwang): Currently this is only used for development of new features.
// Apply this to Android and ChromeOS as well where hardware secure decryption
// is already available.
const base::Feature kHardwareSecureDecryption{
    "HardwareSecureDecryption", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature kWakeLockOptimisationHiddenMuted{
    "kWakeLockOptimisationHiddenMuted", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables handling of hardware media keys for controlling media.
const base::Feature kHardwareMediaKeyHandling {
  "HardwareMediaKeyHandling",
#if BUILDFLAG(IS_CHROMEOS_ASH) || defined(OS_WIN) || defined(OS_MAC) || \
    BUILDFLAG(USE_MPRIS)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// Enables a platform-specific resolution cutoff for prioritizing platform
// decoders over software decoders or vice-versa.
const base::Feature kResolutionBasedDecoderPriority{
    "ResolutionBasedDecoderPriority", base::FEATURE_DISABLED_BY_DEFAULT};

// Forces use of hardware (platform) video decoders in
// `media::DecoderSelector`.
const base::Feature kForceHardwareVideoDecoders{
    "ForceHardwareVideoDecoders", base::FEATURE_DISABLED_BY_DEFAULT};

// Forces use of hardware (platform) audio decoders in
// `media::DecoderSelector`.
const base::Feature kForceHardwareAudioDecoders{
    "ForceHardwareAudioDecoders", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables low-delay video rendering in media pipeline on "live" stream.
const base::Feature kLowDelayVideoRenderingOnLiveStream{
    "low-delay-video-rendering-on-live-stream",
    base::FEATURE_ENABLED_BY_DEFAULT};

// Whether the autoplay policy should ignore Web Audio. When ignored, the
// autoplay policy will be hardcoded to be the legacy one on based on the
// platform
const base::Feature kAutoplayIgnoreWebAudio{"AutoplayIgnoreWebAudio",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

// Whether we should show a setting to disable autoplay policy.
const base::Feature kAutoplayDisableSettings{"AutoplayDisableSettings",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_ANDROID)
// Should we allow video playback to use an overlay if it's not needed for
// security?  Normally, we'd always want to allow this, except as part of the
// power testing A/B experiment.  https://crbug.com/1081346 .
const base::Feature kAllowNonSecureOverlays{"AllowNonSecureOverlays",
                                            base::FEATURE_ENABLED_BY_DEFAULT};

// Enable a gesture to make the media controls expaned into the display cutout.
// TODO(beccahughes): Remove this.
const base::Feature kMediaControlsExpandGesture{
    "MediaControlsExpandGesture", base::FEATURE_ENABLED_BY_DEFAULT};

// An experimental feature to enable persistent-license type support in MediaDrm
// when using Encrypted Media Extensions (EME) API.
// TODO(xhwang): Remove this after feature launch. See http://crbug.com/493521
const base::Feature kMediaDrmPersistentLicense{
    "MediaDrmPersistentLicense", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables MediaDrmOriginIdManager to provide preprovisioned origin IDs for
// MediaDrmBridge. If disabled, MediaDrmBridge will get unprovisioned origin IDs
// which will trigger provisioning process after MediaDrmBridge is created.
const base::Feature kMediaDrmPreprovisioning{"MediaDrmPreprovisioning",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

// Determines if MediaDrmOriginIdManager should attempt to pre-provision origin
// IDs at startup (whenever a profile is loaded). Also used by tests that
// disable it so that the tests can setup before pre-provisioning is done.
// Note: Has no effect if kMediaDrmPreprovisioning feature is disabled.
const base::Feature kMediaDrmPreprovisioningAtStartup{
    "MediaDrmPreprovisioningAtStartup", base::FEATURE_ENABLED_BY_DEFAULT};

// Prevents using SurfaceLayer for videos. This is meant to be used by embedders
// that cannot support SurfaceLayer at the moment.
const base::Feature kDisableSurfaceLayerForVideo{
    "DisableSurfaceLayerForVideo", base::FEATURE_DISABLED_BY_DEFAULT};

// Enable picture in picture web api for android.
const base::Feature kPictureInPictureAPI{"PictureInPictureAPI",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

// Enables CanPlayType() (and other queries) for HLS MIME types. Note that
// disabling this also causes navigation to .m3u8 files to trigger downloading
// instead of playback.
const base::Feature kCanPlayHls{"CanPlayHls", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables the use of MediaPlayerRenderer for HLS playback. When disabled,
// HLS manifests will fail to load (triggering source fallback or load error).
const base::Feature kHlsPlayer{"HlsPlayer", base::FEATURE_ENABLED_BY_DEFAULT};

// When enabled, Playing media sessions will request audio focus from the
// Android system.
const base::Feature kRequestSystemAudioFocus{"RequestSystemAudioFocus",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

// Use the (hacky) AudioManager.getOutputLatency() call to get the estimated
// hardware latency for a stream for OpenSLES playback.  This is normally not
// needed, except for some Android TV devices.
const base::Feature kUseAudioLatencyFromHAL{"UseAudioLatencyFromHAL",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

// Enable pooling of SharedImageVideo objects for use by MCVD, to save a hop to
// the GPU main thread during VideoFrame construction.
const base::Feature kUsePooledSharedImageVideoProvider{
    "UsePooledSharedImageVideoProvider", base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(OS_ANDROID)

#if defined(OS_CHROMEOS) && BUILDFLAG(USE_CHROMEOS_MEDIA_ACCELERATION)
// Enable the hardware-accelerated direct video decoder instead of the one
// needing the VdaVideoDecoder adapter. This flag is used mainly as a
// chrome:flag for developers debugging issues. TODO(b/159825227): remove when
// the direct video decoder is fully launched.
const base::Feature kUseChromeOSDirectVideoDecoder{
    "UseChromeOSDirectVideoDecoder", base::FEATURE_ENABLED_BY_DEFAULT};

// ChromeOS has one of two VideoDecoder implementations active based on
// SoC/board specific configurations that are sent via command line flags. This
// switch allows using the non default implementation for testing.
// TODO(b/159825227): remove when the "old" video decoder is fully launched.
const base::Feature kUseAlternateVideoDecoderImplementation{
    "UseAlternateVideoDecoderImplementation",
    base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_CHROMEOS) && BUILDFLAG(USE_CHROMEOS_MEDIA_ACCELERATION)

#if defined(OS_WIN)
// Does NV12->NV12 video copy on the main thread right before the texture's
// used by GL.
const base::Feature kDelayCopyNV12Textures{"DelayCopyNV12Textures",
                                           base::FEATURE_ENABLED_BY_DEFAULT};

// Enables DirectShow GetPhotoState implementation
// Created to act as a kill switch by disabling it, in the case of the
// resurgence of https://crbug.com/722038
const base::Feature kDirectShowGetPhotoState{"DirectShowGetPhotoState",
                                             base::FEATURE_ENABLED_BY_DEFAULT};

// Includes Infrared cameras in the list returned for EnumerateDevices() on
// Windows.
const base::Feature kIncludeIRCamerasInDeviceEnumeration{
    "IncludeIRCamerasInDeviceEnumeration", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables asynchronous H264 HW encode acceleration using Media Foundation for
// Windows.
const base::Feature kMediaFoundationAsyncH264Encoding{
    "MediaFoundationAsyncH264Encoding", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables AV1 decode acceleration for Windows.
const base::Feature MEDIA_EXPORT kMediaFoundationAV1Decoding{
    "MediaFoundationAV1Decoding", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables MediaFoundation based video capture
const base::Feature kMediaFoundationVideoCapture{
    "MediaFoundationVideoCapture", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables MediaFoundation based video capture with D3D11
const base::Feature kMediaFoundationD3D11VideoCapture{
    "MediaFoundationD3D11VideoCapture", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables VP8 decode acceleration for Windows.
const base::Feature MEDIA_EXPORT kMediaFoundationVP8Decoding{
    "MediaFoundationVP8Decoding", base::FEATURE_DISABLED_BY_DEFAULT};

// Use the AUDCLNT_STREAMOPTIONS_RAW option on WASAPI input audio streams in
// combination with  the IAudioClient2::SetClientProperties() API.
// The audio stream is a 'raw' stream that bypasses all signal processing except
// for endpoint specific, always-on processing in the Audio Processing Object
// (APO), driver, and hardware.
// https://docs.microsoft.com/en-us/windows/win32/api/audioclient/ne-audioclient-audclnt_streamoptions
const base::Feature MEDIA_EXPORT kWasapiRawAudioCapture{
    "WASAPIRawAudioCapture", base::FEATURE_DISABLED_BY_DEFAULT};

#endif  // defined(OS_WIN)

#if defined(OS_MAC)
// Controls whether the next version mac capturer, including power improvements,
// zero copy operation, and other improvements, is active.
const base::Feature MEDIA_EXPORT kAVFoundationCaptureV2{
    "AVFoundationCaptureV2", base::FEATURE_ENABLED_BY_DEFAULT};

// Controls whether or not the V2 capturer exports IOSurfaces for zero-copy.
// This feature only has any effect if kAVFoundationCaptureV2 is also enabled.
const base::Feature MEDIA_EXPORT kAVFoundationCaptureV2ZeroCopy{
    "AVFoundationCaptureV2ZeroCopy", base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(OS_MAC)

#if defined(OS_CHROMEOS)
const base::Feature MEDIA_EXPORT kDeprecateLowUsageCodecs{
    "DeprecateLowUsageCodecs", base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(OS_CHROMEOS)

std::string GetEffectiveAutoplayPolicy(const base::CommandLine& command_line) {
  // Return the autoplay policy set in the command line, if any.
  if (command_line.HasSwitch(switches::kAutoplayPolicy))
    return command_line.GetSwitchValueASCII(switches::kAutoplayPolicy);

  if (base::FeatureList::IsEnabled(media::kUnifiedAutoplay))
    return switches::autoplay::kDocumentUserActivationRequiredPolicy;

// The default value is platform dependent.
#if defined(OS_ANDROID)
  return switches::autoplay::kUserGestureRequiredPolicy;
#else
  return switches::autoplay::kNoUserGestureRequiredPolicy;
#endif
}

// Enables Media Engagement Index recording. This data will be used to determine
// when to bypass autoplay policies. This is recorded on all platforms.
const base::Feature kRecordMediaEngagementScores{
    "RecordMediaEngagementScores", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables Media Engagement Index recording for Web Audio playbacks.
const base::Feature kRecordWebAudioEngagement{"RecordWebAudioEngagement",
                                              base::FEATURE_ENABLED_BY_DEFAULT};

// The following Media Engagement flags are not enabled on mobile platforms:
// - MediaEngagementBypassAutoplayPolicies: enables the Media Engagement Index
//   data to be esude to override autoplay policies. An origin with a high MEI
//   will be allowed to autoplay.
// - PreloadMediaEngagementData: enables a list of origins to be considered as
//   having a high MEI until there is enough local data to determine the user's
//   preferred behaviour.
#if defined(OS_ANDROID) || defined(OS_IOS)
const base::Feature kMediaEngagementBypassAutoplayPolicies{
    "MediaEngagementBypassAutoplayPolicies", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature kPreloadMediaEngagementData{
    "PreloadMediaEngagementData", base::FEATURE_DISABLED_BY_DEFAULT};
#else
const base::Feature kMediaEngagementBypassAutoplayPolicies{
    "MediaEngagementBypassAutoplayPolicies", base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature kPreloadMediaEngagementData{
    "PreloadMediaEngagementData", base::FEATURE_ENABLED_BY_DEFAULT};
#endif

const base::Feature kMediaEngagementHTTPSOnly{
    "MediaEngagementHTTPSOnly", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables experimental local learning for media. Used in the context of media
// capabilities only. Adds reporting only; does not change media behavior.
const base::Feature kMediaLearningExperiment{"MediaLearningExperiment",
                                             base::FEATURE_DISABLED_BY_DEFAULT};

// Enables the general purpose media machine learning framework. Adds reporting
// only; does not change media behavior.
const base::Feature kMediaLearningFramework{"MediaLearningFramework",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

// Enables the smoothness prediction experiment.  Requires
// kMediaLearningFramework to be enabled also, else it does nothing.
const base::Feature kMediaLearningSmoothnessExperiment{
    "MediaLearningSmoothnessExperiment", base::FEATURE_DISABLED_BY_DEFAULT};

// Enable the prototype global optimization of tuneables via finch.  See
// media/base/tuneable.h for how to create tuneable parameters.
const base::Feature kMediaOptimizer{"JointMediaOptimizer",
                                    base::FEATURE_DISABLED_BY_DEFAULT};

// Enable aggregate power measurement for media playback.
const base::Feature kMediaPowerExperiment{"MediaPowerExperiment",
                                          base::FEATURE_DISABLED_BY_DEFAULT};

// Enable WebRTC actions for the Media Session API.
const base::Feature kMediaSessionWebRTC{"MediaSessionWebRTC",
                                        base::FEATURE_DISABLED_BY_DEFAULT};

// Enables flash to be ducked by audio focus. This is enabled on Chrome OS which
// has audio focus enabled.
const base::Feature kAudioFocusDuckFlash {
  "AudioFocusDuckFlash",
#if BUILDFLAG(IS_CHROMEOS_ASH)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

// Only affects Android. Suspends a media session when audio focus is lost; when
// this setting is disabled, an Android media session will not be suspended when
// Audio focus is lost. This is used by Cast which sometimes needs to drive
// multiple media sessions.
const base::Feature kAudioFocusLossSuspendMediaSession{
    "AudioFocusMediaSession", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables the internal Media Session logic without enabling the Media Session
// service.
const base::Feature kInternalMediaSession {
  "InternalMediaSession",
#if defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

const base::Feature kKaleidoscope{"Kaleidoscope",
                                  base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature kKaleidoscopeInMenu{"KaleidoscopeInMenu",
                                        base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature kKaleidoscopeForceShowFirstRunExperience{
    "KaleidoscopeForceShowFirstRunExperience",
    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature kUseFakeDeviceForMediaStream{
    "use-fake-device-for-media-stream", base::FEATURE_DISABLED_BY_DEFAULT};

// Makes VideoCadenceEstimator use Bresenham-like algorithm for frame cadence
// estimations.
const base::Feature kBresenhamCadence{"BresenhamCadence",
                                      base::FEATURE_DISABLED_BY_DEFAULT};

bool IsVideoCaptureAcceleratedJpegDecodingEnabled() {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableAcceleratedMjpegDecode)) {
    return false;
  }
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kUseFakeMjpegDecodeAccelerator)) {
    return true;
  }
#if BUILDFLAG(IS_CHROMEOS_ASH)
  return true;
#endif
  return false;
}

}  // namespace media



//https://raw.githubusercontent.com/chromium/chromium/master/components/autofill_assistant/browser/switches.cc
// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/autofill_assistant/browser/switches.h"

namespace autofill_assistant {
namespace switches {

// Sets the API key to be used instead of Chrome's default key when sending
// requests to the backend.
const char kAutofillAssistantServerKey[] = "autofill-assistant-key";

// Overrides the default backend URL.
const char kAutofillAssistantUrl[] = "autofill-assistant-url";

// Disables authentication when set to false. This is only useful
// during development, as prod instances require authentication.
const char kAutofillAssistantAuth[] = "autofill-assistant-auth";

}  // namespace switches
}  // namespace autofill_assistant







//https://raw.githubusercontent.com/chromium/chromium/master/ash/constants/ash_switches.cc
// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/constants/ash_switches.h"

#include <string>

#include "base/command_line.h"
#include "base/metrics/field_trial.h"

namespace chromeos {
namespace switches {

namespace {

// The memory pressure thresholds selection which is used to decide whether and
// when a memory pressure event needs to get fired.
const char kMemoryPressureExperimentName[] = "ChromeOSMemoryPressureHandling";
const char kMemoryPressureHandlingOff[] = "memory-pressure-off";

// Controls CrOS GaiaId migration for tests ("" is default).
const char kTestCrosGaiaIdMigration[] = "test-cros-gaia-id-migration";

// Value for kTestCrosGaiaIdMigration indicating that migration is started (i.e.
// all stored user keys will be converted to GaiaId)
const char kTestCrosGaiaIdMigrationStarted[] = "started";

}  // namespace

// Please keep the order of these switches synchronized with the header file
// (i.e. in alphabetical order).

const char kAggressiveCacheDiscardThreshold[] = "aggressive-cache-discard";

// If this flag is passed, failed policy fetches will not cause profile
// initialization to fail. This is useful for tests because it means that
// tests don't have to mock out the policy infrastructure.
const char kAllowFailedPolicyFetchForTest[] =
    "allow-failed-policy-fetch-for-test";

// Allows remote attestation (RA) in dev mode for testing purpose. Usually RA
// is disabled in dev mode because it will always fail. However, there are cases
// in testing where we do want to go through the permission flow even in dev
// mode. This can be enabled by this flag.
const char kAllowRAInDevMode[] = "allow-ra-in-dev-mode";

// Specifies whether an app launched in kiosk mode was auto launched with zero
// delay. Used in order to properly restore auto-launched state during session
// restore flow.
const char kAppAutoLaunched[] = "app-auto-launched";

// Path for app's OEM manifest file.
const char kAppOemManifestFile[] = "app-mode-oem-manifest";

// Signals ARC support status on this device. This can take one of the
// following three values.
// - none: ARC is not installed on this device. (default)
// - installed: ARC is installed on this device, but not officially supported.
//   Users can enable ARC only when Finch experiment is turned on.
// - officially-supported: ARC is installed and supported on this device. So
//   users can enable ARC via settings etc.
const char kArcAvailability[] = "arc-availability";

// DEPRECATED: Please use --arc-availability=installed.
// Signals the availability of the ARC instance on this device.
const char kArcAvailable[] = "arc-available";

// Flag that forces ARC data be cleaned on each start.
const char kArcDataCleanupOnStart[] = "arc-data-cleanup-on-start";

// Flag that disables ARC app sync flow that installs some apps silently. Used
// in autotests to resolve racy conditions.
const char kArcDisableAppSync[] = "arc-disable-app-sync";

// Flag that disables ARC download provider that prevents extra content to be
// downloaded and installed in context of Play Store and GMS Core.
const char kArcDisableDownloadProvider[] = "arc-disable-download-provider";

// Flag to enables an experiment to allow users to turn on 64-bit support in
// native bridge on systems that have such support available but not yet enabled
// by default.
const char kArcEnableNativeBridge64BitSupportExperiment[] =
    "arc-enable-native-bridge-64bit-support-experiment";

// Used in autotest to disable GMS-core caches which is on by default.
const char kArcDisableGmsCoreCache[] = "arc-disable-gms-core-cache";

// Flag that disables ARC locale sync with Android container. Used in autotest
// to prevent conditions when certain apps, including Play Store may get
// restarted. Restarting Play Store may cause random test failures. Enabling
// this flag would also forces ARC container to use 'en-US' as a locale and
// 'en-US,en' as preferred languages.
const char kArcDisableLocaleSync[] = "arc-disable-locale-sync";

// Used to disable GMS scheduling of media store periodic indexing and corpora
// maintenance tasks. Used in performance tests to prevent running during
// testing which can cause unstable results or CPU not idle pre-test failures.
const char kArcDisableMediaStoreMaintenance[] =
    "arc-disable-media-store-maintenance";

// Flag that disables ARC Play Auto Install flow that installs set of predefined
// apps silently. Used in autotests to resolve racy conditions.
const char kArcDisablePlayAutoInstall[] = "arc-disable-play-auto-install";

// Used for development of Android app that are included into ARC as system
// default apps in order to be able to install them via adb.
const char kArcDisableSystemDefaultApps[] = "arc-disable-system-default-apps";

// Flag that forces the OptIn ui to be shown. Used in tests.
const char kArcForceShowOptInUi[] = "arc-force-show-optin-ui";

// Flag that enables developer options needed to generate an ARC Play Auto
// Install roster. Used manually by developers.
const char kArcGeneratePlayAutoInstall[] = "arc-generate-play-auto-install";

// Write ARC++ install events to chrome log for integration test.
const char kArcInstallEventChromeLogForTests[] =
    "arc-install-event-chrome-log-for-tests";

// Used in autotest to specifies how to handle packages cache. Can be
// copy - copy resulting packages.xml to the temporary directory.
// skip-copy - skip initial packages cache setup and copy resulting packages.xml
//             to the temporary directory.
const char kArcPackagesCacheMode[] = "arc-packages-cache-mode";

// Used in autotest to forces Play Store auto-update state. Can be
// on - auto-update is forced on.
// off - auto-update is forced off.
const char kArcPlayStoreAutoUpdate[] = "arc-play-store-auto-update";

// Set the scale for ARC apps. This is in DPI. e.g. 280 DPI is ~ 1.75 device
// scale factor.
// See
// https://source.android.com/compatibility/android-cdd#3_7_runtime_compatibility
// for list of supported DPI values.
const char kArcScale[] = "arc-scale";

// Defines how to start ARC. This can take one of the following values:
// - always-start automatically start with Play Store UI support.
// - always-start-with-no-play-store automatically start without Play Store UI.
// If it is not set, then ARC is started in default mode.
const char kArcStartMode[] = "arc-start-mode";

// Sets ARC Terms Of Service hostname url for testing.
const char kArcTosHostForTests[] = "arc-tos-host-for-tests";

// Sets the mode of operation for ureadahead during ARCVM boot. If this switch
// is not set, ARCVM ureadahead will check for the presence and age of pack
// file and reads ahead files to page cache for improved boot performance.
// generate - used during Android PFQ data collector to pre-generate pack file
//            and upload to Google Cloud as build artifact for CrOS build image.
// disabled - used for test purpose to disable ureadahead during ARCVM boot.
const char kArcVmUreadaheadMode[] = "arcvm-ureadahead-mode";

// If this flag is set, it indicates that this device is a "Cellular First"
// device. Cellular First devices use cellular telephone data networks as
// their primary means of connecting to the internet.
// Setting this flag has two consequences:
// 1. Cellular data roaming will be enabled by default.
// 2. UpdateEngine will be instructed to allow auto-updating over cellular
//    data connections.
const char kCellularFirst[] = "cellular-first";

// Default large wallpaper to use for kids accounts (as path to trusted,
// non-user-writable JPEG file).
const char kChildWallpaperLarge[] = "child-wallpaper-large";

// Default small wallpaper to use for kids accounts (as path to trusted,
// non-user-writable JPEG file).
const char kChildWallpaperSmall[] = "child-wallpaper-small";

// Forces CrOS region value.
const char kCrosRegion[] = "cros-region";

// Control regions data load ("" is default).
const char kCrosRegionsMode[] = "cros-regions-mode";

// "Hide" value for kCrosRegionsMode (VPD values are hidden).
const char kCrosRegionsModeHide[] = "hide";

// "Override" value for kCrosRegionsMode (region's data is read first).
const char kCrosRegionsModeOverride[] = "override";

// Controls if AuthSession API should be used when interacting with cryptohomed.
const char kCryptohomeUseAuthSession[] = "cryptohome-use-authsession";

// Indicates that the wallpaper images specified by
// kAshDefaultWallpaper{Large,Small} are OEM-specific (i.e. they are not
// downloadable from Google).
const char kDefaultWallpaperIsOem[] = "default-wallpaper-is-oem";

// Default large wallpaper to use (as path to trusted, non-user-writable JPEG
// file).
const char kDefaultWallpaperLarge[] = "default-wallpaper-large";

// Default small wallpaper to use (as path to trusted, non-user-writable JPEG
// file).
const char kDefaultWallpaperSmall[] = "default-wallpaper-small";

// Time in seconds before a machine at OOBE is considered derelict.
const char kDerelictDetectionTimeout[] = "derelict-detection-timeout";

// Time in seconds before a derelict machines starts demo mode.
const char kDerelictIdleTimeout[] = "derelict-idle-timeout";

// Disables android user data wipe on opt out.
const char kDisableArcDataWipe[] = "disable-arc-data-wipe";

// Disables ARC Opt-in verification process and ARC is enabled by default.
const char kDisableArcOptInVerification[] = "disable-arc-opt-in-verification";

// Disables the Chrome OS demo.
const char kDisableDemoMode[] = "disable-demo-mode";

// If this switch is set, the device cannot be remotely disabled by its owner.
const char kDisableDeviceDisabling[] = "disable-device-disabling";

// Disable encryption migration for user's cryptohome to run latest Arc.
const char kDisableEncryptionMigration[] = "disable-encryption-migration";

// Disables fine grained time zone detection.
const char kDisableFineGrainedTimeZoneDetection[] =
    "disable-fine-grained-time-zone-detection";

// Disables GAIA services such as enrollment and OAuth session restore. Used by
// 'fake' telemetry login.
const char kDisableGaiaServices[] = "disable-gaia-services";

// Disables HID-detection OOBE screen.
const char kDisableHIDDetectionOnOOBEForTesting[] =
    "disable-hid-detection-on-oobe";

// Avoid doing expensive animations upon login.
const char kDisableLoginAnimations[] = "disable-login-animations";

// Disables requests for an enterprise machine certificate during attestation.
const char kDisableMachineCertRequest[] = "disable-machine-cert-request";

// Disables the ChromeVox hint idle detection in OOBE, which can lead to
// unexpected behavior during tests.
const char kDisableOOBEChromeVoxHintTimerForTesting[] =
    "disable-oobe-chromevox-hint-timer-for-testing";

// Enables the ChromeVox hint in OOBE for dev mode. This flag is used
// to override the default dev mode behavior of disabling the feature.
// If both kEnableOOBEChromeVoxHintForDevMode and
// kDisableOOBEChromeVoxHintTimerForTesting are present, the ChromeVox hint
// will be disabled, since the latter flag takes precedence over the former.
const char kEnableOOBEChromeVoxHintForDevMode[] =
    "enable-oobe-chromevox-hint-timer-for-dev-mode";

// Disables per-user timezone.
const char kDisablePerUserTimezone[] = "disable-per-user-timezone";

// Disables rollback option on reset screen.
const char kDisableRollbackOption[] = "disable-rollback-option";

// Disables client certificate authentication on the sign-in frame on the Chrome
// OS sign-in profile.
// TODO(https://crbug.com/844022): Remove this flag when reaching endpoints that
// request client certs does not hang anymore when there is no system token yet.
const char kDisableSigninFrameClientCerts[] =
    "disable-signin-frame-client-certs";

// Disables volume adjust sound.
const char kDisableVolumeAdjustSound[] = "disable-volume-adjust-sound";

// DEPRECATED. Please use --arc-availability=officially-supported.
// Enables starting the ARC instance upon session start.
const char kEnableArc[] = "enable-arc";

// Enables ARCVM.
const char kEnableArcVm[] = "enable-arcvm";

// Enables ARCVM realtime VCPU feature.
const char kEnableArcVmRtVcpu[] = "enable-arcvm-rt-vcpu";

// Enables the Cast Receiver.
const char kEnableCastReceiver[] = "enable-cast-receiver";

// Enables consumer kiosk mode for Chrome OS.
const char kEnableConsumerKiosk[] = "enable-consumer-kiosk";

// Enables encryption migration for user's cryptohome to run latest Arc.
const char kEnableEncryptionMigration[] = "enable-encryption-migration";

// Enables sharing assets for installed default apps.
const char kEnableExtensionAssetsSharing[] = "enable-extension-assets-sharing";

// Enables the use of 32-bit Houdini library for ARM binary translation.
const char kEnableHoudini[] = "enable-houdini";

// Enables the use of 64-bit Houdini library for ARM binary translation.
const char kEnableHoudini64[] = "enable-houdini64";

// Enables the use of 32-bit NDK translation library for ARM binary translation.
const char kEnableNdkTranslation[] = "enable-ndk-translation";

// Enables the use of 64-bit NDK translation library for ARM binary translation.
const char kEnableNdkTranslation64[] = "enable-ndk-translation64";

// Enables request of tablet site (via user agent override).
const char kEnableRequestTabletSite[] = "enable-request-tablet-site";

// Enables tablet form factor.
const char kEnableTabletFormFactor[] = "enable-tablet-form-factor";

// Enables the touch calibration option in MD settings UI for valid touch
// displays.
const char kEnableTouchCalibrationSetting[] =
    "enable-touch-calibration-setting";

// Enables touchpad three-finger-click as middle button.
const char kEnableTouchpadThreeFingerClick[] =
    "enable-touchpad-three-finger-click";

// Disables ARC for managed accounts.
const char kEnterpriseDisableArc[] = "enterprise-disable-arc";

// Whether to enable forced enterprise re-enrollment.
const char kEnterpriseEnableForcedReEnrollment[] =
    "enterprise-enable-forced-re-enrollment";

// Whether to enable initial enterprise enrollment.
const char kEnterpriseEnableInitialEnrollment[] =
    "enterprise-enable-initial-enrollment";

// Whether to enable PSM (private set membership) queries.
const char kEnterpriseEnablePsm[] = "enterprise-enable-psm";

// Enables the zero-touch enterprise enrollment flow.
const char kEnterpriseEnableZeroTouchEnrollment[] =
    "enterprise-enable-zero-touch-enrollment";

// Power of the power-of-2 initial modulus that will be used by the
// auto-enrollment client. E.g. "4" means the modulus will be 2^4 = 16.
const char kEnterpriseEnrollmentInitialModulus[] =
    "enterprise-enrollment-initial-modulus";

// Power of the power-of-2 maximum modulus that will be used by the
// auto-enrollment client.
const char kEnterpriseEnrollmentModulusLimit[] =
    "enterprise-enrollment-modulus-limit";

// Interval in seconds between Chrome reading external metrics from
// /var/lib/metrics/uma-events.
const char kExternalMetricsCollectionInterval[] =
    "external-metrics-collection-interval";

// Name of a subdirectory of the main external web apps directory which
// additional web apps configs should be loaded from. Used to load
// device-specific web apps.
const char kExtraWebAppsDir[] = "extra-web-apps-dir";

// An absolute path to the chroot hosting the DriveFS to use. This is only used
// when running on Linux, i.e. when IsRunningOnChromeOS() returns false.
const char kFakeDriveFsLauncherChrootPath[] =
    "fake-drivefs-launcher-chroot-path";

// A relative path to socket to communicat with the fake DriveFS launcher within
// the chroot specified by kFakeDriveFsLauncherChrootPath. This is only used
// when running on Linux, i.e. when IsRunningOnChromeOS() returns false.
const char kFakeDriveFsLauncherSocketPath[] =
    "fake-drivefs-launcher-socket-path";

// Specifies number of recommended (fake) ARC apps during user onboarding.
// App descriptions are generated locally instead of being fetched from server.
// Limited to ChromeOS-on-linux and test images only.
const char kFakeArcRecommendedAppsForTesting[] =
    "fake-arc-recommended-apps-for-testing";

// Fingerprint sensor location indicates the physical sensor's location. The
// value is a string with possible values: "power-button-top-left",
// "keyboard-bottom-left", keyboard-bottom-right", "keyboard-top-right".
const char kFingerprintSensorLocation[] = "fingerprint-sensor-location";

// Specifies the device's form factor. If provided, this flag overrides the
// value from the LSB release info. Possible values are: "CHROMEBASE",
// "CHROMEBIT", "CHROMEBOOK", "REFERENCE", "CHROMEBOX"
const char kFormFactor[] = "form-factor";

// Passed to Chrome the first time that it's run after the system boots.
// Not passed on restart after sign out.
const char kFirstExecAfterBoot[] = "first-exec-after-boot";

// Forces developer tools availability, no matter what values the enterprise
// policies DeveloperToolsDisabled and DeveloperToolsAvailability are set to.
const char kForceDevToolsAvailable[] = "force-devtools-available";

// Forces first-run UI to be shown for every login.
const char kForceFirstRunUI[] = "force-first-run-ui";

// Force enables the Happiness Tracking System for the device. This ignores
// user profile check and time limits and shows the notification every time
// for any type of user. Should be used only for testing.
const char kForceHappinessTrackingSystem[] = "force-happiness-tracking-system";

// Forces Hardware ID check (happens during OOBE) to fail. Should be used only
// for testing.
const char kForceHWIDCheckFailureForTest[] =
    "force-hwid-check-failure-for-test";

// Usually in browser tests the usual login manager bringup is skipped so that
// tests can change how it's brought up. This flag disables that.
const char kForceLoginManagerInTests[] = "force-login-manager-in-tests";

// Force system compositor mode when set.
const char kForceSystemCompositorMode[] = "force-system-compositor-mode";

// Indicates that the browser is in "browse without sign-in" (Guest session)
// mode. Should completely disable extensions, sync and bookmarks.
const char kGuestSession[] = "bwsi";

// Large wallpaper to use in guest mode (as path to trusted, non-user-writable
// JPEG file).
const char kGuestWallpaperLarge[] = "guest-wallpaper-large";

// Small wallpaper to use in guest mode (as path to trusted, non-user-writable
// JPEG file).
const char kGuestWallpaperSmall[] = "guest-wallpaper-small";

// If set, the system is a Chromebook with a "standard Chrome OS keyboard",
// which generally means one with a Search key in the standard Caps Lock
// location above the Left Shift key. It should be unset for Chromebooks with
// both Search and Caps Lock keys (e.g. stout) and for devices like Chromeboxes
// that only use external keyboards.
const char kHasChromeOSKeyboard[] = "has-chromeos-keyboard";

// Defines user homedir. This defaults to primary user homedir.
const char kHomedir[] = "homedir";

// If set, the "ignore_dev_conf" field in StartArcVmRequest message will
// consequently be set such that all development configuration directives in
// /usr/local/vms/etc/arcvm_dev.conf will be ignored during ARCVM start.
const char kIgnoreArcVmDevConf[] = "ignore-arcvm-dev-conf";

// If true, profile selection in UserManager will always return active user's
// profile.
// TODO(nkostlyev): http://crbug.com/364604 - Get rid of this switch after we
// turn on multi-profile feature on ChromeOS.
const char kIgnoreUserProfileMappingForTests[] =
    "ignore-user-profile-mapping-for-tests";

// Decreases delay in uploading installation event logs for integration test.
const char kInstallLogFastUploadForTests[] =
    "install-log-fast-upload-for-tests";

// If set, the Chrome settings will not expose the option to enable crostini
// unless the enable-experimental-kernel-vm-support flag is set in
// chrome://flags
const char kKernelnextRestrictVMs[] = "kernelnext-restrict-vms";

// If this switch is set, then ash-chrome will pass additional arguments when
// launching lacros-chrome. The string '####' is used as a delimiter. Example:
// --lacros-chrome-additional-args="--foo=5####--bar=/tmp/dir name". Will
// result in two arguments passed to lacros-chrome:
//   --foo=5
//   --bar=/tmp/dir name
const char kLacrosChromeAdditionalArgs[] = "lacros-chrome-additional-args";

// Additional environment variables set for lacros-chrome. The string '####' is
// used as a delimiter. For example:
// --lacros-chrome-additional-env=WAYLAND_DEBUG=client####FOO=bar
// will enable Wayland protocol logging and set FOO=bar.
const char kLacrosChromeAdditionalEnv[] = "lacros-chrome-additional-env";

// If this switch is set, then ash-chrome will exec the lacros-chrome binary
// from the indicated path rather than from component updater. Note that the
// path should be to a directory that contains a binary named 'chrome'.
const char kLacrosChromePath[] = "lacros-chrome-path";

// If set, ash-chrome will drop a Unix domain socket to wait for a process to
// connect to it, and the connection will be used to request file descriptors
// from ash-chrome, and when the process forks to start a lacros-chrome, the
// obtained file descriptor will be used by lacros-chrome to set up the mojo
// connection with ash-chrome. There are mainly two use cases:
// 1. Test launcher to run browser tests in testing environment.
// 2. A terminal to start lacros-chrome with a debugger.
const char kLacrosMojoSocketForTesting[] = "lacros-mojo-socket-for-testing";

// Enables Chrome-as-a-login-manager behavior.
const char kLoginManager[] = "login-manager";

// Specifies the profile to use once a chromeos user is logged in.
// This parameter is ignored if user goes through login screen since user_id
// hash defines which profile directory to use.
// In case of browser restart within active session this parameter is used
// to pass user_id hash for primary user.
const char kLoginProfile[] = "login-profile";

// Specifies the user which is already logged in.
const char kLoginUser[] = "login-user";

// Determines the URL to be used when calling the backend.
const char kMarketingOptInUrl[] = "marketing-opt-in-url";

// Enables natural scroll by default.
const char kNaturalScrollDefault[] = "enable-natural-scroll-default";

// An optional comma-separated list of IDs of apps that can be used to take
// notes. If unset, a hardcoded list is used instead.
const char kNoteTakingAppIds[] = "note-taking-app-ids";

// Allows the eula url to be overridden for tests.
const char kOobeEulaUrlForTests[] = "oobe-eula-url-for-tests";

// Indicates that the first user run flow (sequence of OOBE screens after the
// first user login) should show tablet mode centric screens, even if the device
// is not in tablet mode.
const char kOobeForceTabletFirstRun[] = "oobe-force-tablet-first-run";

// Indicates that a guest session has been started before OOBE completion.
const char kOobeGuestSession[] = "oobe-guest-session";

// Skips all other OOBE pages after user login.
const char kOobeSkipPostLogin[] = "oobe-skip-postlogin";

// Skip to login screen.
const char kOobeSkipToLogin[] = "oobe-skip-to-login";

// Interval at which we check for total time on OOBE.
const char kOobeTimerInterval[] = "oobe-timer-interval";

// Allows the timezone to be overridden on the marketing opt-in screen.
const char kOobeTimezoneOverrideForTests[] = "oobe-timezone-override-for-tests";

// SAML assertion consumer URL, used to detect when Gaia-less SAML flows end
// (e.g. for SAML managed guest sessions)
// TODO(984021): Remove when URL is sent by DMServer.
const char kPublicAccountsSamlAclUrl[] = "public-accounts-saml-acl-url";

// If set to "true", the profile requires policy during restart (policy load
// must succeed, otherwise session restart should fail).
const char kProfileRequiresPolicy[] = "profile-requires-policy";

// The rlz ping delay (in seconds) that overwrites the default value.
const char kRlzPingDelay[] = "rlz-ping-delay";

// The switch added by session_manager daemon when chrome crashes 3 times or
// more within the first 60 seconds on start.
// See BrowserJob::ExportArgv in platform2/login_manager/browser_job.cc.
const char kSafeMode[] = "safe-mode";

// Password change url for SAML users.
// TODO(941489): Remove when the bug is fixed.
const char kSamlPasswordChangeUrl[] = "saml-password-change-url";

// Used for overriding the time limit imposed by the policies
// SAMLOfflineSigninTimeLimit & GaiaOfflineSigninTimeLimitDays when testing.
// TODO(crbug.com/1177416): Clean up once testing is complete
const char kOfflineSignInTimeLimitInSecondsOverrideForTesting[] =
    "offline-signin-timelimit-in-seconds-override-for-testing";

// Used for overriding the preference set by the policy
// kSamlLockScreenReauthenticationEnabled to true.
// TODO(crbug.com/1177416): Clean up once testing is complete
const char kSamlLockScreenReauthenticationEnabledOverrideForTesting[] =
    "saml-lockscreen-reauthentication-enabled-override-for-testing";

// New modular design for the shelf with apps separated into a hotseat UI and
// smaller shelf in clamshell mode.
const char kShelfHotseat[] = "shelf-hotseat";

// App window previews when hovering over the shelf.
const char kShelfHoverPreviews[] = "shelf-hover-previews";

// The name of the per-model directory which contains per-region
// subdirectories with regulatory label files for this model.
// The per-model directories (if there are any) are located under
// "/usr/share/chromeos-assets/regulatory_labels/".
const char kRegulatoryLabelDir[] = "regulatory-label-dir";

// If true, the developer tool overlay will be shown for the login/lock screen.
// This makes it easier to test layout logic.
const char kShowLoginDevOverlay[] = "show-login-dev-overlay";

// Enables OOBE UI Debugger for ease of navigation between screens during manual
// testing. Limited to ChromeOS-on-linux and test images only.
const char kShowOobeDevOverlay[] = "show-oobe-dev-overlay";

// Enables OOBE testing API for tast tests.
const char kEnableOobeTestAPI[] = "enable-oobe-test-api";

// Specifies directory for screenshots taken with OOBE UI Debugger.
const char kOobeScreenshotDirectory[] = "oobe-screenshot-dir";

// Disables online sign-in enforcement in tast tests.
const char kSkipForceOnlineSignInForTesting[] =
    "skip-force-online-signin-for-testing";

// Specifies directory for the Telemetry System Web Extension.
const char kTelemetryExtensionDirectory[] = "telemetry-extension-dir";

// Enables testing for encryption migration UI.
const char kTestEncryptionMigrationUI[] = "test-encryption-migration-ui";

// Enables the wallpaper picker to fetch images from the test server.
const char kTestWallpaperServer[] = "test-wallpaper-server";

// Overrides Tether with stub service. Provide integer arguments for the number
// of fake networks desired, e.g. 'tether-stub=2'.
const char kTetherStub[] = "tether-stub";

// Tells the Chromebook to scan for a tethering host even if there is already a
// wired connection. This allows end-to-end tests to be deployed over ethernet
// without that connection preventing scans and thereby blocking the testing of
// cases with no preexisting connection. Should be used only for testing.
const char kTetherHostScansIgnoreWiredConnections[] =
    "tether-host-scans-ignore-wired-connections";

// Shows all Bluetooth devices in UI (System Tray/Settings Page.)
const char kUnfilteredBluetoothDevices[] = "unfiltered-bluetooth-devices";

// Used to tell the policy infrastructure to not let profile initialization
// complete until policy is manually set by a test. This is used to provide
// backward compatibility with a few tests that incorrectly use the
// synchronously-initialized login profile to run their tests - do not add new
// uses of this flag.
const char kWaitForInitialPolicyFetchForTest[] =
    "wait-for-initial-policy-fetch-for-test";

// Prevents any CPU restrictions being set on the ARC container. Only meant to
// be used by tests as some tests may time out if the ARC container is
// throttled.
const char kDisableArcCpuRestriction[] = "disable-arc-cpu-restriction";

// If this switch is passed, the device policy DeviceMinimumVersion
// assumes that the device has reached Auto Update Expiration. This is useful
// for testing the policy behaviour on the DUT.
const char kUpdateRequiredAueForTest[] = "aue-reached-for-update-required-test";

// Uses fake ml service impl to simulate CrOS ml service daemon. This should
// only be used for lacros_chrome_browsertests that requires ml service.
const char kUseFakeMLServiceForTest[] = "use-fake-ml-service-for-test";

// Enables configuring the OEM Device Requsition in the OOBE.
const char kEnableRequisitionEdits[] = "enable-requisition-edits";

bool MemoryPressureHandlingEnabled() {
  if (base::FieldTrialList::FindFullName(kMemoryPressureExperimentName) ==
      kMemoryPressureHandlingOff) {
    return false;
  }
  return true;
}

bool IsAuthSessionCryptohomeEnabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kCryptohomeUseAuthSession);
}

bool IsGaiaIdMigrationStarted() {
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (!command_line->HasSwitch(kTestCrosGaiaIdMigration))
    return false;

  return command_line->GetSwitchValueASCII(kTestCrosGaiaIdMigration) ==
         kTestCrosGaiaIdMigrationStarted;
}

bool IsCellularFirstDevice() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(kCellularFirst);
}

bool IsSigninFrameClientCertsEnabled() {
  return !base::CommandLine::ForCurrentProcess()->HasSwitch(
      kDisableSigninFrameClientCerts);
}

bool ShouldShowShelfHoverPreviews() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(kShelfHoverPreviews);
}

bool ShouldTetherHostScansIgnoreWiredConnections() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kTetherHostScansIgnoreWiredConnections);
}

bool ShouldSkipOobePostLogin() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(kOobeSkipPostLogin);
}

bool IsTabletFormFactor() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kEnableTabletFormFactor);
}

bool IsGaiaServicesDisabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kDisableGaiaServices);
}

bool IsArcCpuRestrictionDisabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kDisableArcCpuRestriction);
}

bool IsUnfilteredBluetoothDevicesEnabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kUnfilteredBluetoothDevices);
}

bool ShouldOobeUseTabletModeFirstRun() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kOobeForceTabletFirstRun);
}

bool IsAueReachedForUpdateRequiredForTest() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kUpdateRequiredAueForTest);
}

bool IsOOBEChromeVoxHintTimerDisabledForTesting() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kDisableOOBEChromeVoxHintTimerForTesting);
}

bool IsOOBEChromeVoxHintEnabledForDevMode() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kEnableOOBEChromeVoxHintForDevMode);
}

bool IsDeviceRequisitionConfigurable() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kEnableRequisitionEdits);
}

}  // namespace switches
}  // namespace chromeos






//https://raw.githubusercontent.com/chromium/chromium/master/chromecast/net/net_switches.cc

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chromecast/net/net_switches.h"

namespace switches {

// Url for network connectivity checking. Default is
// "https://clients3.google.com/generate_204".
const char kConnectivityCheckUrl[] = "connectivity-check-url";

// List of network interfaces to ignore. Ignored interfaces will not be used
// for network connectivity.
const char kNetifsToIgnore[] = "netifs-to-ignore";

}  // namespace switches





//https://raw.githubusercontent.com/chromium/chromium/master/ash/public/cpp/ash_switches.cc
// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/public/cpp/ash_switches.h"

#include "base/command_line.h"
#include "base/numerics/ranges.h"
#include "base/strings/string_number_conversions.h"

namespace {
// Max and min number of seconds that must pass between showing user contextual
// nudges when override switch is set.
constexpr base::TimeDelta kAshContextualNudgesMinInterval =
    base::TimeDelta::FromSeconds(0);
constexpr base::TimeDelta kAshContextualNudgesMaxInterval =
    base::TimeDelta::FromSeconds(60);
}  // namespace

namespace ash {
namespace switches {

// Clear the fast ink buffer upon creation. This is needed on some devices that
// do not zero out new buffers.
const char kAshClearFastInkBuffer[] = "ash-clear-fast-ink-buffer";

// Force the pointer (cursor) position to be kept inside root windows.
const char kAshConstrainPointerToRoot[] = "ash-constrain-pointer-to-root";

// Overrides the minimum time that must pass between showing user contextual
// nudges. Unit of time is in seconds.
const char kAshContextualNudgesInterval[] = "ash-contextual-nudges-interval";

// Reset contextual nudge shown count on login.
const char kAshContextualNudgesResetShownCount[] =
    "ash-contextual-nudges-reset-shown-count";

// Enable keyboard shortcuts useful for debugging.
const char kAshDebugShortcuts[] = "ash-debug-shortcuts";

// Enable keyboard shortcuts used by developers only.
const char kAshDeveloperShortcuts[] = "ash-dev-shortcuts";

// Disable the Touch Exploration Mode. Touch Exploration Mode will no longer be
// turned on automatically when spoken feedback is enabled when this flag is
// set.
const char kAshDisableTouchExplorationMode[] =
    "ash-disable-touch-exploration-mode";

// Enable cursor motion blur.
const char kAshEnableCursorMotionBlur[] = "ash-enable-cursor-motion-blur";

// Enables key bindings to scroll magnified screen.
const char kAshEnableMagnifierKeyScroller[] =
    "ash-enable-magnifier-key-scroller";

// Enables the palette on every display, instead of only the internal one.
const char kAshEnablePaletteOnAllDisplays[] =
    "ash-enable-palette-on-all-displays";

// If the flag is present, it indicates 1) the device has accelerometer and 2)
// the device is a convertible device or a tablet device (thus is capable of
// entering tablet mode). If this flag is not set, then the device is not
// capable of entering tablet mode. For example, Samus has accelerometer, but
// is not a covertible or tablet, thus doesn't have this flag set, thus can't
// enter tablet mode.
const char kAshEnableTabletMode[] = "enable-touchview";

// Enable the wayland server.
const char kAshEnableWaylandServer[] = "enable-wayland-server";

// Enables the stylus tools next to the status area.
const char kAshForceEnableStylusTools[] = "force-enable-stylus-tools";

// Forces the status area to allow collapse/expand regardless of the current
// state.
const char kAshForceStatusAreaCollapsible[] = "force-status-area-collapsible";

// Power button position includes the power button's physical display side and
// the percentage for power button center position to the display's
// width/height in landscape_primary screen orientation. The value is a JSON
// object containing a "position" property with the value "left", "right",
// "top", or "bottom". For "left" and "right", a "y" property specifies the
// button's center position as a fraction of the display's height (in [0.0,
// 1.0]) relative to the top of the display. For "top" and "bottom", an "x"
// property gives the position as a fraction of the display's width relative to
// the left side of the display.
const char kAshPowerButtonPosition[] = "ash-power-button-position";

// Enables required things for the selected UI mode, regardless of whether the
// Chromebook is currently in the selected UI mode.
const char kAshUiMode[] = "force-tablet-mode";

// Values for the kAshUiMode flag.
const char kAshUiModeClamshell[] = "clamshell";
const char kAshUiModeTablet[] = "touch_view";

// Hides notifications that are irrelevant to Chrome OS device factory testing,
// such as battery level updates.
const char kAshHideNotificationsForFactory[] =
    "ash-hide-notifications-for-factory";

// Enables the heads-up display for tracking touch points.
const char kAshTouchHud[] = "ash-touch-hud";

// The physical position info of the side volume button while in landscape
// primary screen orientation. The value is a JSON object containing a "region"
// property with the value "keyboard", "screen" and a "side" property with the
// value "left", "right", "top", "bottom".
const char kAshSideVolumeButtonPosition[] = "ash-side-volume-button-position";

// (Most) Chrome OS hardware reports ACPI power button releases correctly.
// Standard hardware reports releases immediately after presses.  If set, we
// lock the screen or shutdown the system immediately in response to a press
// instead of displaying an interactive animation.
const char kAuraLegacyPowerButton[] = "aura-legacy-power-button";

// Enables Shelf Dimming for ChromeOS.
const char kEnableDimShelf[] = "enable-dim-shelf";

// Enables compositing-based throttling
const char kEnableCompositingBasedThrottling[] =
    "enable-compositing-based-throttling";

// If set, tablet-like power button behavior (i.e. tapping the button turns the
// screen off) is used even if the device is in laptop mode.
const char kForceTabletPowerButton[] = "force-tablet-power-button";

// Sets the throttle fps for compositor frame submission.
const char kFrameThrottleFps[] = "frame-throttle-fps";

// Whether this device has an internal stylus.
const char kHasInternalStylus[] = "has-internal-stylus";

// Draws a circle at each touch point, similar to the Android OS developer
// option "Show taps".
const char kShowTaps[] = "show-taps";

// Chromebases' touchscreens can be used to wake from suspend, unlike the
// touchscreens on other Chrome OS devices. If set, the touchscreen is kept
// enabled while the screen is off so that it can be used to turn the screen
// back on after it has been turned off for inactivity but before the system has
// suspended.
const char kTouchscreenUsableWhileScreenOff[] =
    "touchscreen-usable-while-screen-off";

// Hides all Message Center notification popups (toasts). Used for testing.
const char kSuppressMessageCenterPopups[] = "suppress-message-center-popups";

// If set, the device will be forced to stay in clamshell UI mode but screen
// auto rotation will be supported. E.g, chromebase device Dooly.
const char kSupportsClamshellAutoRotation[] =
    "supports-clamshell-auto-rotation";

base::Optional<base::TimeDelta> ContextualNudgesInterval() {
  int numeric_cooldown_time;
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          kAshContextualNudgesInterval) &&
      base::StringToInt(
          base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
              kAshContextualNudgesInterval),
          &numeric_cooldown_time)) {
    base::TimeDelta cooldown_time =
        base::TimeDelta::FromSeconds(numeric_cooldown_time);
    cooldown_time =
        base::ClampToRange(cooldown_time, kAshContextualNudgesMinInterval,
                           kAshContextualNudgesMaxInterval);
    return base::Optional<base::TimeDelta>(cooldown_time);
  }
  return base::nullopt;
}

bool ContextualNudgesResetShownCount() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kAshContextualNudgesResetShownCount);
}

bool IsUsingShelfAutoDim() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(kEnableDimShelf);
}

bool IsCompositingBasedThrottlingEnabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kEnableCompositingBasedThrottling);
}

bool ShouldClearFastInkBuffer() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kAshClearFastInkBuffer);
}

}  // namespace switches
}  // namespace ash





//https://raw.githubusercontent.com/chromium/chromium/master/ui/wm/core/wm_core_switches.cc
// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/wm/core/wm_core_switches.h"

#include "base/command_line.h"

namespace wm {
namespace switches {

// If present animations are disabled.
const char kWindowAnimationsDisabled[] = "wm-window-animations-disabled";

}  // namespace switches
}  // namespace wm




//https://raw.githubusercontent.com/chromium/chromium/master/ui/display/display_switches.cc

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/display/display_switches.h"
#include "build/build_config.h"
#include "build/chromeos_buildflags.h"

namespace switches {

// TODO(rjkroege): Some of these have an "ash" prefix. When ChromeOS startup
// scripts have been updated, the leading "ash" prefix should be removed.

// Enables software based mirroring.
const char kEnableSoftwareMirroring[] = "ash-enable-software-mirroring";

// Crash the browser at startup if the display's color profile does not match
// the forced color profile. This is necessary on Mac because Chrome's pixel
// output is always subject to the color conversion performed by the operating
// system. On all other platforms, this is a no-op.
const char kEnsureForcedColorProfile[] = "ensure-forced-color-profile";

// Force all monitors to be treated as though they have the specified color
// profile. Accepted values are "srgb" and "generic-rgb" (currently used by Mac
// layout tests) and "color-spin-gamma24" (used by layout tests).
const char kForceDisplayColorProfile[] = "force-color-profile";

// Force rastering to take place in the specified color profile. Accepted values
// are the same as for the kForceDisplayColorProfile case above.
const char kForceRasterColorProfile[] = "force-raster-color-profile";

// Overrides the device scale factor for the browser UI and the contents.
const char kForceDeviceScaleFactor[] = "force-device-scale-factor";

// Sets a window size, optional position, and optional scale factor.
// "1024x768" creates a window of size 1024x768.
// "100+200-1024x768" positions the window at 100,200.
// "1024x768*2" sets the scale factor to 2 for a high DPI display.
// "800,0+800-800x800" for two displays at 800x800 resolution.
// "800,0+800-800x800,0+1600-800x800" for three displays at 800x800 resolution.
const char kHostWindowBounds[] = "ash-host-window-bounds";

// Specifies the layout mode and offsets for the secondary display for
// testing. The format is "<t|r|b|l>,<offset>" where t=TOP, r=RIGHT,
// b=BOTTOM and L=LEFT. For example, 'r,-100' means the secondary display
// is positioned on the right with -100 offset. (above than primary)
const char kSecondaryDisplayLayout[] = "secondary-display-layout";

// Specifies the initial screen configuration, or state of all displays, for
// FakeDisplayDelegate, see class for format details.
const char kScreenConfig[] = "screen-config";

// Uses the 1st display in --ash-host-window-bounds as internal display.
// This is for debugging on linux desktop.
const char kUseFirstDisplayAsInternal[] = "use-first-display-as-internal";

#if BUILDFLAG(IS_CHROMEOS_ASH)
// Enables unified desktop mode.
const char kEnableUnifiedDesktop[] = "ash-enable-unified-desktop";
#endif

}  // namespace switches




//https://raw.githubusercontent.com/chromium/chromium/master/media/webrtc/webrtc_switches.cc

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "media/webrtc/webrtc_switches.h"

#include "base/command_line.h"
#include "build/build_config.h"

namespace switches {

// Override the default minimum starting volume of the Automatic Gain Control
// algorithm in WebRTC used with audio tracks from getUserMedia.
// The valid range is 12-255. Values outside that range will be clamped
// to the lowest or highest valid value inside WebRTC.
// TODO(tommi): Remove this switch when crbug.com/555577 is fixed.
const char kAgcStartupMinVolume[] = "agc-startup-min-volume";

}  // namespace switches

namespace features {

// Enables multichannel capture audio to be processed without downmixing in the
// WebRTC audio processing module.
const base::Feature kWebRtcEnableCaptureMultiChannelApm{
    "WebRtcEnableCaptureMultiChannelApm", base::FEATURE_ENABLED_BY_DEFAULT};

// Kill-switch allowing deactivation of the support for 48 kHz internal
// processing in the WebRTC audio processing module when running on an ARM
// platform.
const base::Feature kWebRtcAllow48kHzProcessingOnArm{
    "WebRtcAllow48kHzProcessingOnArm", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables the WebRTC Agc2 digital adaptation with WebRTC Agc1 analog
// adaptation. Feature for http://crbug.com/873650. Is sent to WebRTC.
const base::Feature kWebRtcHybridAgc{"WebRtcHybridAgc",
                                     base::FEATURE_DISABLED_BY_DEFAULT};

}  // namespace features



//https://raw.githubusercontent.com/chromium/chromium/master/base/i18n/base_i18n_switches.cc
// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/i18n/base_i18n_switches.h"

namespace switches {

// Force the UI to a specific direction. Valid values are "ltr" (left-to-right)
// and "rtl" (right-to-left).
const char kForceUIDirection[]   = "force-ui-direction";

// Force the text rendering to a specific direction. Valid values are "ltr"
// (left-to-right) and "rtl" (right-to-left). Only tested meaningfully with
// RTL.
const char kForceTextDirection[] = "force-text-direction";

const char kForceDirectionLTR[]  = "ltr";
const char kForceDirectionRTL[]  = "rtl";

}  // namespace switches

