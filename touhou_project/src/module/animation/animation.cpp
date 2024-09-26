/**
 * @file animation.cpp
 * @brief
 */

#include "include/module/animation/animation.h"

#include <fstream>
#include <sstream>

#include "include/component/animator/animator.h"
#include "include/core/engine/engine.h"
#include "include/manager/time_manager.h"
#include "include/manager/asset_manager.h"

Animation::Animation()
  : owner_(nullptr),
    atlas_(nullptr),
    current_idx_(0),
    accumulated_time_(0.f),
    is_finish_(false) {}

Animation::Animation(const Animation& other)
  : Base(other),
    owner_(nullptr),
    frame_vector_(other.frame_vector_),
    atlas_(other.atlas_),
    current_idx_(other.current_idx_),
    accumulated_time_(other.accumulated_time_),
    is_finish_(other.is_finish_) {}

Animation::~Animation() = default;

/**
 * @brief Tick에 맞춰서 Frame 변경하는 메서드
 */
void Animation::FinalTick() {
  if (is_finish_)
    return;

  // TimeManager로부터 누적 시간 확인
  accumulated_time_ += TimeManager::Get()->float_delta_time();

  // 진행된 시간이 현재 프레임의 duration_을 초과하면 다음 프레임으로 변경
  if (frame_vector_[current_idx_].duration_ < accumulated_time_) {
    accumulated_time_ -= frame_vector_[current_idx_].duration_;
    ++current_idx_;

    // 1 사이클을 전부 돈 경우 마지막 프레임으로 세팅
    if (current_idx_ >= static_cast<int>(frame_vector_.size())) {
      current_idx_ = static_cast<int>(frame_vector_.size()) - 1;
      is_finish_ = true;
    }
  }
}

/**
 * @brief 특정 위치를 기준점으로 Animation을 화면에 표시하는 메서드
 */
void Animation::Render() const {
  // atlas가 존재하지 않으면 render 불가능
  assert(atlas_);

  const HDC device_context = Engine::Get()->GetBackDC();

  // Animation을 소유한 오브젝트의 위치
  const Vector2 position = owner_->owner()->position();

  BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
  const AnimationFrame& current_frame = frame_vector_[current_idx_];

  AlphaBlend(device_context,
             static_cast<int>(position.x()
                              - current_frame.slice_.x() / 2.f + current_frame.offset_.x()),
             static_cast<int>(position.y()
                              - current_frame.slice_.y() / 2.f + current_frame.offset_.y()),
             static_cast<int>(current_frame.slice_.x()),
             static_cast<int>(current_frame.slice_.y()),
             atlas_->dc_handle(),
             static_cast<int>(current_frame.left_top_.x()),
             static_cast<int>(current_frame.left_top_.y()),
             static_cast<int>(current_frame.slice_.x()),
             static_cast<int>(current_frame.slice_.y()),
             bf);
}

/**
 * @brief 파일에 애니메이션 정보를 기록하는 메서드
 * @param folder_path
 * TODO(KHJ): IO Manager를 따로 만들어서 로드만 시키는 것도 괜찮을 듯?
 */
void Animation::Save(const filesystem::path& folder_path) {
  string animation_name = name();
  filesystem::path file_path = folder_path / (animation_name + ".animation");

  ofstream file(file_path, ios::out | ios::binary);
  if (!file) {
    throw runtime_error("Failed to open file for writing: " + file_path.string());
  }

  // UTF-8 BOM 추가
  file.write("\xEF\xBB\xBF", 3);

  // 애니메이션 이름 기재
  file << "[ANIMATION_NAME]\n";
  file << animation_name << "\n\n";

  // 참조하던 아틀라스 텍스쳐 정보 기재
  file << "[ATLAS_TEXTURE]\n";
  if (atlas_) {
    file << atlas_->name() << "\n";
    file << atlas_->relative_path() << "\n";
  } else {
    file << "None\n";
    file << "None\n";
  }
  file << "\n";

  // 각 프레임별 데이터 기재
  file << "[FRAME_DATA]\n";
  file << "Frame_Count " << frame_vector_.size() << "\n\n";

  for (size_t i = 0; i < frame_vector_.size(); ++i) {
    file << "Frame_Index " << i << "\n";
    file << fixed << setprecision(6); // 부동 소수점 정밀도 설정
    file << "Left_Top    " << frame_vector_[i].left_top_.x()
        << " " << frame_vector_[i].left_top_.y() << "\n";
    file << "Offset      " << frame_vector_[i].offset_.x()
        << " " << frame_vector_[i].offset_.y() << "\n";
    file << "Slice       " << frame_vector_[i].slice_.x()
        << " " << frame_vector_[i].slice_.y() << "\n";
    file << "Duration    " << frame_vector_[i].duration_ << "\n\n";
  }

  if (!file) {
    throw runtime_error("Error occurred while writing to file: " + file_path.string());
  }
}

/**
 * @brief 파일로부터 애니메이션 정보를 불러오는 메서드
 * @param file_path
 */
void Animation::Load(const filesystem::path& file_path) {
  ifstream file(file_path, ios::in | ios::binary);
  if (!file) {
    throw runtime_error("Failed to open file: " + file_path.string());
  }

  string line;
  while (getline(file, line)) {
    // BOM 체크 및 제거 (UTF-8 파일의 경우)
    if (line.size() >= 3 && line[0] == '\xEF' && line[1] == '\xBB' && line[2] == '\xBF') {
      line.erase(0, 3);
    }

    istringstream iss(line);
    string key;
    iss >> key;

    if (key == "[ANIMATION_NAME]") {
      string name;
      iss >> name;
      set_name(name);
    } else if (key == "[ATLAS_TEXTURE]") {
      string texture_key;
      string relative_path;
      iss >> texture_key >> relative_path;
      if (texture_key != "None") {
        atlas_ = AssetManager::Get()->LoadTexture(texture_key, relative_path);
      }
    } else if (key == "[FRAME_DATA]") {
      AnimationFrame frame;
      while (getline(file, line) && !line.empty()) {
        istringstream frame_iss(line);
        string frame_key;
        frame_iss >> frame_key;
        float temp_x, temp_y;

        if (frame_key == "Left_Top") {
          frame_iss >> temp_x >> temp_y;
          frame.left_top_.set_x(temp_x);
          frame.left_top_.set_y(temp_y);
        } else if (frame_key == "Offset") {
          frame_iss >> temp_x >> temp_y;
          frame.offset_.set_x(temp_x);
          frame.offset_.set_y(temp_y);
        } else if (frame_key == "Slice") {
          frame_iss >> temp_x >> temp_y;
          frame.slice_.set_x(temp_x);
          frame.slice_.set_y(temp_y);
        } else if (frame_key == "Duration") {
          frame_iss >> frame.duration_;
          frame_vector_.push_back(frame);
        }
      }
    }
  }
}

/**
 * @brief Animation의 정보를 가지고 Animation 구조체를 세팅하는 메서드
 * @param info
 */
void Animation::Create(const AnimationDescription& info) {
  set_name(info.name_);
  atlas_ = info.atlas_;

  for (int i = 0; i < info.frame_count_; ++i) {
    AnimationFrame frame = {};
    frame.left_top_.set_x(static_cast<float>(info.start_left_top_.x() + info.slice_size_.x() * i));
    frame.left_top_.set_y(static_cast<float>(info.start_left_top_.y()));
    frame.slice_ = info.slice_size_;
    frame.duration_ = 1.f / static_cast<float>(info.fps_);
    frame_vector_.push_back(frame);
  }
}

/**
 * @brief 1 사이클 순회가 완료된 Animation을 다시 돌 수 있도록 리셋하는 메서드
 */
void Animation::Reset() {
  is_finish_ = false;
  accumulated_time_ = 0.f;
  current_idx_ = 0;
}

// end of animation.cpp