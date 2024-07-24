/**
 * @file animation.cpp
 * @brief
 */

#include "include/module/animation/animation.h"

#include "include/component/animator/animator.h"
#include "include/core/engine/engine.h"
#include "include/manager/time_manager.h"

Animation::Animation()
    : animator_(nullptr), atlas_(nullptr), current_idx_(0), accumulated_time_(0.f),
      is_finish_(false) {}

Animation::Animation(const Animation &other)
    : Base(other), animator_(nullptr), frame_vector_(other.frame_vector_),
      atlas_(other.atlas_), current_idx_(other.current_idx_),
      accumulated_time_(other.accumulated_time_), is_finish_(other.is_finish_) {
}

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
  Vector2 position = animator_->owner()->position();

  TransparentBlt(
      device_context,
      static_cast<int>(
          position.x() -
          static_cast<float>(frame_vector_[current_idx_].slice_.x()) / 2.f +
          frame_vector_[current_idx_].offset_.x()),
      static_cast<int>(position.y() -
                       frame_vector_[current_idx_].slice_.y() / 2.f +
                       frame_vector_[current_idx_].offset_.y()),
      static_cast<int>(frame_vector_[current_idx_].slice_.x()),
      static_cast<int>(frame_vector_[current_idx_].slice_.y()),
      atlas_->dc_handle(),
      static_cast<int>(frame_vector_[current_idx_].left_top_.x()),
      static_cast<int>(frame_vector_[current_idx_].left_top_.y()),
      static_cast<int>(frame_vector_[current_idx_].slice_.x()),
      static_cast<int>(frame_vector_[current_idx_].slice_.y()),
      RGB(255, 0, 255));
}

/**
 * @brief 파일에 애니메이션 정보를 기록하는 메서드
 * @param folder_path
 * TODO(KHJ): IO Manager를 따로 만들어서 로드만 시키는 것도 괜찮을 듯?
 */
void Animation::Save(const wstring &folder_path) const {
  // TODO(KHJ): name 받아올 수 있도록 할 것
  wstring animation_name = L"foo";
  const wstring file_path = folder_path + animation_name + L".animation";

  FILE *file = nullptr;
  _wfopen_s(&file, file_path.c_str(), L"w");

  // 애니메이션 이름 기재
  (void)fwprintf_s(file, L"[ANIMATION_NAME]\n");
  (void)fwprintf_s(file, animation_name.c_str());
  (void)fwprintf_s(file, L"\n\n");

  // 참조하던 아틀라스 텍스쳐 정보 기재
  (void)fwprintf_s(file, L"[ATLAS_TEXTURE]\n");

  if (atlas_) {
    (void)fwprintf_s(file, L"%s\n", atlas_->key().c_str());
    (void)fwprintf_s(file, L"%s\n", atlas_->relative_path().c_str());
  } else {
    (void)fwprintf_s(file, L"None\n");
    (void)fwprintf_s(file, L"None\n");
  }
  (void)fwprintf_s(file, L"\n");

  // 각 프레임별 데이터 기재
  (void)fwprintf_s(file, L"[FRAME_DATA]\n");
  (void)fwprintf_s(file, L"Frame_Count %d\n\n",
                   static_cast<int>(frame_vector_.size()));

  for (int i = 0; i < static_cast<int>(frame_vector_.size()); ++i) {
    (void)fwprintf_s(file, L"Frame_Index %d\n", i);
    (void)fwprintf_s(file, L"Left_Top    %f %f\n",
                     frame_vector_[i].left_top_.x(),
                     frame_vector_[i].left_top_.y());
    (void)fwprintf_s(file, L"Offset      %f %f\n", frame_vector_[i].offset_.x(),
                     frame_vector_[i].offset_.y());
    (void)fwprintf_s(file, L"Slice       %f %f\n", frame_vector_[i].slice_.x(),
                     frame_vector_[i].slice_.y());
    (void)fwprintf_s(file, L"Duration    %f\n\n", frame_vector_[i].duration_);
  }

  (void)fclose(file);
}

/**
 * @brief 파일로부터 애니메이션 정보를 불러오는 메서드
 * @param file_path
 */
void Animation::Load(const wstring &file_path) {
  FILE *file = nullptr;
  _wfopen_s(&file, file_path.c_str(), L"r");

  wchar_t file_read_buf[255] = {};
  while (true) {
    if (fwscanf_s(file, L"%s", file_read_buf, 255) == EOF)
      break;

    wstring str = file_read_buf;

    if (str == L"[ANIMATION_NAME]") {
      (void)fwscanf_s(file, L"%s", file_read_buf, 255);
      set_name(file_read_buf);
    } else if (str == L"[ATLAS_TEXTURE]") {
      wstring strKey, strRelativePath;
      (void)fwscanf_s(file, L"%s", file_read_buf, 255);
      strKey = file_read_buf;

      (void)fwscanf_s(file, L"%s", file_read_buf, 255);
      strRelativePath = file_read_buf;

      if (strKey != L"None") {
        atlas_ = AssetManager::Get()->LoadTexture(strKey, strRelativePath);
      }
    } else if (str == L"[FRAME_DATA]") {
      wchar_t frame_buffer[255] = {};

      AnimationFrame frame = {};

      while (true) {
        if (fwscanf_s(file, L"%s", frame_buffer, 255) == EOF) {
          break;
        }

        if (!wcscmp(L"Left_Top", frame_buffer)) {
          (void)fwscanf_s(file, L"%f %f", frame.left_top_.x(),
                          frame.left_top_.y());
        } else if (!wcscmp(L"Offset", frame_buffer)) {
          (void)fwscanf_s(file, L"%f %f", frame.offset_.x(), frame.offset_.y());
        } else if (!wcscmp(L"Slice", frame_buffer)) {
          (void)fwscanf_s(file, L"%f %f", frame.slice_.x(), frame.slice_.y());
        } else if (!wcscmp(L"Duration", frame_buffer)) {
          (void)fwscanf_s(file, L"%f", frame.duration_);
          frame_vector_.push_back(frame);
        }
      }
    }
  }

  (void)fclose(file);
}

/**
 * @brief Animation의 정보를 가지고 Animation 구조체를 세팅하는 메서드
 * @param info
 */
void Animation::Create(const AnimationDescription &info) {
  set_name(info.name_);
  atlas_ = info.atlas_;

  for (int i = 0; i < info.frame_count_; ++i) {
    AnimationFrame frame = {};
    frame.left_top_.set_x(static_cast<float>(info.start_left_top_.x() +
                                             info.slice_size_.x() * i));
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
