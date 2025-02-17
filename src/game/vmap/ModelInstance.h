/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _MODELINSTANCE_H_
#define _MODELINSTANCE_H_

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>
#include <G3D/AABox.h>
#include <G3D/Ray.h>

#include "Platform/Define.h"

namespace VMAP
{
    class WorldModel;
    struct AreaInfo;
    struct LocationInfo;

    enum ModelFlags
    {
        MOD_M2 = 1,
        MOD_WORLDSPAWN = 1 << 1,
        MOD_HAS_BOUND = 1 << 2
    };

    class ModelSpawn
    {
        public:
            // mapID, tileX, tileY, Flags, ID, Pos, Rot, Scale, Bound_lo, Bound_hi, name
            uint32 flags;
            uint16 adtId;
            uint32 ID;
            G3D::Vector3 iPos;
            G3D::Vector3 iRot;
            float iScale;
            G3D::AABox iBound;
            std::string name;
            bool operator==(const ModelSpawn& other) const { return ID == other.ID; }
            // uint32 hashCode() const { return ID; }
            // temp?
            const G3D::AABox& getBounds() const { return iBound; }


            static bool readFromFile(FILE* rf, ModelSpawn& spawn);
            static bool writeToFile(FILE* wf, const ModelSpawn& spawn);
    };

    class ModelInstance: public ModelSpawn
    {
        public:
            ModelInstance(): iInvScale(0), iModel(nullptr) {}
            ModelInstance(const ModelSpawn& spawn, WorldModel* model);
            void setUnloaded() { iModel = nullptr; }
            bool intersectRay(const G3D::Ray& pRay, float& pMaxDist, bool pStopAtFirstHit, bool ignoreM2Model = false) const;
            void intersectPoint(const G3D::Vector3& p, AreaInfo& info) const;
            bool isUnderModel(G3D::Vector3 const& p, float* outDist = nullptr, float* inDist = nullptr) const;
            bool GetLocationInfo(const G3D::Vector3& p, LocationInfo& info) const;
            bool GetLiquidLevel(const G3D::Vector3& p, LocationInfo& info, float& liqHeight) const;
        protected:
            G3D::Matrix3 iInvRot;
            float iInvScale;
            WorldModel* iModel;

#ifdef MMAP_GENERATOR
        public:
            WorldModel* const getWorldModel();
#endif
    };
} // namespace VMAP

#endif // _MODELINSTANCE
