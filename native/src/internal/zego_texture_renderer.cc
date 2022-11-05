//
//  zego_texture_renderer.cc
//  zego_express_engine_plugin
//
//  Created by Patrick Fu on 2022/9/30.
//

#include "zego_texture_renderer.h"
#include "zego_utils.h"

namespace zego::cocos {

ZegoTextureRenderer::ZegoTextureRenderer() { texture_id_ = GetNextSequence(); }

void ZegoTextureRenderer::SetJsController(const se::Value &controller) {
    js_controller_ = std::make_shared<se::Value>(controller);
}

int64_t ZegoTextureRenderer::TextureId() { return texture_id_; }

uint32_t ZegoTextureRenderer::Width() { return width_; }

uint32_t ZegoTextureRenderer::Height() { return height_; }

uint32_t ZegoTextureRenderer::Rotation() { return rotation_; }

ZegoVideoFlipMode ZegoTextureRenderer::FlipMode() { return flip_mode_; }

void ZegoTextureRenderer::UpdateFrameBuffer(uint8_t *data, uint32_t data_length,
                                            ZEGO::EXPRESS::ZegoVideoFrameParam param,
                                            ZegoVideoFlipMode flip_mode) {
    //    std::lock_guard<std::mutex> lock(mutex_);
    
    width_ = param.width;
    height_ = param.height;
    rotation_ = param.rotation;
    flip_mode_ = flip_mode;

    if (js_controller_) {
//        auto &texture_id = texture_id_;
//        auto &js_controller = js_controller_;
//        RunOnCocosThread([texture_id, js_controller, data] {
        se::ScriptEngine::getInstance()->clearException();
        se::AutoHandleScope hs;
        se::Value method;
        if (js_controller_->toObject()->getProperty("updateRendererFrameBuffer", &method)) {
            if (!method.isObject()) {
                return;
            }
            auto func = method.toObject();
            if (!func->isFunction()) {
                return;
            }

            se::Value js_texture_id;
            nativevalue_to_se(texture_id_, js_texture_id, nullptr);
            se::Value js_data;
            nativevalue_to_se(data, js_data, nullptr);

            se::ValueArray args;
            args.push_back(js_texture_id);
            args.push_back(js_data);

            func->call(args, js_controller_->toObject());
        }
//        });
    }
}

//uint8_t *ZegoTextureRenderer::GetFrameBuffer() {
//    const std::lock_guard<std::mutex> lock(mutex_);
//    return buffer_.data();
//}

} // namespace zego::cocos
