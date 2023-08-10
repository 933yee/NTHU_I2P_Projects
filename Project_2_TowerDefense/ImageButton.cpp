#include <functional>
#include <memory>

#include "Collider.hpp"
#include "GameEngine.hpp"
#include "Image.hpp"
#include "ImageButton.hpp"
#include "Point.hpp"
#include "Resources.hpp"
#include "ArmySelectScene.hpp"

extern Engine::ImageButton* rec_intro[10];

namespace Engine {
	ImageButton::ImageButton(std::string img, std::string imgIn, float x, float y, float w, float h, float anchorX, float anchorY) :
		Image(img, x, y, w, h, anchorX, anchorY), imgOut(Resources::GetInstance().GetBitmap(img)), imgIn(Resources::GetInstance().GetBitmap(imgIn)) {
		Point mouse = GameEngine::GetInstance().GetMousePosition();
		mouseIn = Collider::IsPointInBitmap(Point((mouse.x - Position.x) * GetBitmapWidth() / Size.x + Anchor.x * GetBitmapWidth(), (mouse.y - Position.y) * GetBitmapHeight() / Size.y + Anchor.y * GetBitmapHeight()), bmp);
		rec_img1_location = img;
		rec_img2_location = imgIn;
		if (!mouseIn || !Enabled) {	
			bmp = imgOut;
		}else {
			bmp = this->imgIn;
		}
	}
	void ImageButton::SetOnClickCallback(std::function<void(void)> onClickCallback) {
		OnClickCallback = onClickCallback;
	}
	void ImageButton::OnMouseDown(int button, int mx, int my) {
		if ((button & 1) && mouseIn && Enabled) {
			if (OnClickCallback)
				OnClickCallback();
		}
	}
	void ImageButton::OnMouseMove(int mx, int my) {
		std::string tmp1 = rec_img1_location;
		std::string tmp2 = rec_img2_location;
		tmp1.erase(tmp1.size() - 4, 4);
		tmp2.erase(tmp2.size() - 6, 6);
		int id, flag = 0;
		if (tmp1 == tmp2) {
			flag = 1;
			id = rec_img2_location[rec_img2_location.size() - 5] - '0' + 1;
		}
		mouseIn = Collider::IsPointInBitmap(Point((mx - Position.x) * GetBitmapWidth() / Size.x + Anchor.x * GetBitmapWidth(), (my - Position.y) * GetBitmapHeight() / Size.y + Anchor.y * GetBitmapHeight()), bmp);
		if (!mouseIn || !Enabled) {
			if (flag) rec_intro[id]->bmp = rec_intro[id]->imgOut;
			bmp = imgOut;
		}
		else {
			if (flag) rec_intro[id]->bmp = rec_intro[id]->imgIn;
			bmp = imgIn;
		}
	}
}
