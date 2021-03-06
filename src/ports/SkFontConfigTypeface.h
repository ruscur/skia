/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkFontConfigInterface.h"
#include "SkFontDescriptor.h"
#include "SkFontHost_FreeType_common.h"
#include "SkRefCnt.h"
#include "SkStream.h"

class SkFontDescriptor;

class SkTypeface_FCI : public SkTypeface_FreeType {
    sk_sp<SkFontConfigInterface> fFCI;
    SkFontConfigInterface::FontIdentity fIdentity;
    SkString fFamilyName;
    std::unique_ptr<SkFontData> fFontData;

public:
    static SkTypeface_FCI* Create(SkFontConfigInterface* fci,
                                  const SkFontConfigInterface::FontIdentity& fi,
                                  const SkString& familyName,
                                  const SkFontStyle& style)
    {
        return new SkTypeface_FCI(fci, fi, familyName, style);
    }

    static SkTypeface_FCI* Create(std::unique_ptr<SkFontData> data,
                                  SkFontStyle style, bool isFixedPitch)
    {
        return new SkTypeface_FCI(std::move(data), style, isFixedPitch);
    }

    const SkFontConfigInterface::FontIdentity& getIdentity() const {
        return fIdentity;
    }

protected:
    SkTypeface_FCI(SkFontConfigInterface* fci,
                   const SkFontConfigInterface::FontIdentity& fi,
                   const SkString& familyName,
                   const SkFontStyle& style)
            : INHERITED(style, false)
            , fFCI(SkRef(fci))
            , fIdentity(fi)
            , fFamilyName(familyName)
            , fFontData(nullptr) {}

    SkTypeface_FCI(std::unique_ptr<SkFontData> data, SkFontStyle style, bool isFixedPitch)
            : INHERITED(style, isFixedPitch)
            , fFontData(std::move(data))
    {
        SkASSERT(fFontData);
        fIdentity.fTTCIndex = fFontData->getIndex();
    }

    void onGetFamilyName(SkString* familyName) const override { *familyName = fFamilyName; }
    void onGetFontDescriptor(SkFontDescriptor*, bool*) const override;
    SkStreamAsset* onOpenStream(int* ttcIndex) const override;
    SkFontData* onCreateFontData() const override;

private:
    typedef SkTypeface_FreeType INHERITED;
};
