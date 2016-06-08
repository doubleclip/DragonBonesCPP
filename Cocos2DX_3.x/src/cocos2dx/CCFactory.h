#ifndef DRAGONBONES_CC_FACTORY_H
#define DRAGONBONES_CC_FACTORY_H

#include "DragonBonesHeaders.h"
#include "cocos2d.h"

DRAGONBONES_NAMESPACE_BEGIN

class CCFactory final : public BaseFactory
{
public:
    CCFactory();
    ~CCFactory();

private:
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CCFactory);

protected:
    virtual TextureAtlasData* _generateTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const override;
    virtual Armature* _generateArmature(const BuildArmaturePackage& dataPackage) const override;
    virtual Slot* _generateSlot(const BuildArmaturePackage& dataPackage, const SlotDisplayDataSet& slotDisplayDataSet) const override;

public:
    virtual DragonBonesData* loadDragonBonesData(const std::string& filePath, const std::string& dragonBonesName = "");
    virtual TextureAtlasData* loadTextureAtlasData(const std::string& filePath, const std::string& dragonBonesName = "", float scale = 0.f);
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CC_FACTORY_H