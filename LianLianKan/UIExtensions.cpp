#include "LianLianKan.h"


void LianLianKan::resetDiffStyle() {
	ui.easyButton->setStyleSheet(R"(
		QPushButton { background: transparent; border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");
	ui.normalButton->setStyleSheet(R"(
		QPushButton { background: transparent; border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");
	ui.hardButton->setStyleSheet(R"(
		QPushButton { background: transparent; border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");
}

void LianLianKan::selectEasyMode() {
	resetDiffStyle();
	difficulty = easy;
	ui.easyButton->setStyleSheet(R"(
		QPushButton { background-image: url(:/LianLianKan/Images/blue.png); border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");

}

void LianLianKan::selectNormalMode() {
	resetDiffStyle();
	difficulty = normal;
	ui.normalButton->setStyleSheet(R"(
		QPushButton { background-image: url(:/LianLianKan/Images/yellow.png); border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");

}

void LianLianKan::selectHardMode() {
	resetDiffStyle();
	difficulty = hard;
	ui.hardButton->setStyleSheet(R"(
		QPushButton { background-image: url(:/LianLianKan/Images/red.png); border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");

}

void LianLianKan::updateUserInfo() {
	QString user_info = QString::fromUtf16(u"êÇ³Æ£º") + username
		+ "\n" + QString::fromUtf16(u"·ÖÊý£º") + QString::number(score);
	ui.userInfo->setText(user_info);
}

void LianLianKan::drawBlocks(vector<vector<int>> mapVec) {
	int block_count = 0;
	for (int i = 17; i >= 0; i--)
	{
		for (int j = 0; j < 10; j++)
		{
			if (mapVec[j][i] == 0) continue;
			int block_type = mapVec[j][i] - 1;
			Block *_block = new Block(this, block_type, i, j, blockPixmaps[block_type]);
			scene->addItem(_block);
			block_count++;
		}
	}
	remainBlocks = block_count;
}

void LianLianKan::updateItems()
{
	if (remainResorts == 0) {
		ui.resortButton->setEnabled(false);
	}
	else ui.resortButton->setEnabled(true);
	if (remainNavigators == 0) {
		ui.navigateButton->setEnabled(false);
	}
	else ui.navigateButton->setEnabled(true);
}