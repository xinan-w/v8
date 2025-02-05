// Copyright 2018 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if !V8_ENABLE_WEBASSEMBLY
#error This header should only be included if WebAssembly is enabled.
#endif  // !V8_ENABLE_WEBASSEMBLY

#ifndef V8_WASM_WASM_FEATURES_H_
#define V8_WASM_WASM_FEATURES_H_

// The feature flags are declared in their own header.
#include "src/common/globals.h"
#include "src/wasm/wasm-feature-flags.h"

// Features that are always enabled and do not have a flag.
#define FOREACH_WASM_NON_FLAG_FEATURE(V) \
  V(reftypes)                            \
  V(simd)                                \
  V(threads)                             \
  V(return_call)                         \
  V(extended_const)                      \
  V(relaxed_simd)                        \
  V(gc)                                  \
  V(typed_funcref)                       \
  V(js_inlining)

// All features, including features that do not have flags.
#define FOREACH_WASM_FEATURE(V) \
  FOREACH_WASM_FEATURE_FLAG(V)  \
  FOREACH_WASM_NON_FLAG_FEATURE(V)

namespace v8::internal::wasm {

enum WasmFeature {
#define DECL_FEATURE_ENUM(feat, ...) kFeature_##feat,
  FOREACH_WASM_FEATURE(DECL_FEATURE_ENUM)
#undef DECL_FEATURE_ENUM
};

// Enabled or detected features.
class WasmFeatures : public base::EnumSet<WasmFeature> {
 public:
  constexpr WasmFeatures() = default;
  explicit constexpr WasmFeatures(std::initializer_list<WasmFeature> features)
      : EnumSet(features) {}

  // Simplified getters. Use {has_foo()} instead of {contains(kFeature_foo)}.
#define DECL_FEATURE_GETTER(feat, ...) \
  constexpr bool has_##feat() const { return contains(kFeature_##feat); }
  FOREACH_WASM_FEATURE(DECL_FEATURE_GETTER)
#undef DECL_FEATURE_GETTER

  static constexpr const char* name_for_feature(WasmFeature feature) {
    switch (feature) {
#define NAME(feat, ...)              \
  case WasmFeature::kFeature_##feat: \
    return #feat;
      FOREACH_WASM_FEATURE(NAME)
    }
#undef NAME
  }
  static inline constexpr WasmFeatures All();
  static inline constexpr WasmFeatures None();
  static inline constexpr WasmFeatures ForAsmjs();
  // Retuns optional features that are enabled by flags, plus features that are
  // not enabled by a flag and are always on.
  static WasmFeatures FromFlags();
  static V8_EXPORT_PRIVATE WasmFeatures FromIsolate(Isolate*);
  static V8_EXPORT_PRIVATE WasmFeatures
  FromContext(Isolate*, Handle<NativeContext> context);
};

// static
constexpr WasmFeatures WasmFeatures::All() {
#define LIST_FEATURE(feat, ...) kFeature_##feat,
  return WasmFeatures({FOREACH_WASM_FEATURE(LIST_FEATURE)});
#undef LIST_FEATURE
}

// static
constexpr WasmFeatures WasmFeatures::None() { return {}; }

// static
constexpr WasmFeatures WasmFeatures::ForAsmjs() { return {}; }

enum class CompileTimeImport {
  kJsString,
  kTextEncoder,
  kTextDecoder,
  kStringConstants,
};

inline std::ostream& operator<<(std::ostream& os, CompileTimeImport imp) {
  return os << static_cast<int>(imp);
}

using CompileTimeImports = base::EnumSet<CompileTimeImport, int>;

}  // namespace v8::internal::wasm

#endif  // V8_WASM_WASM_FEATURES_H_
