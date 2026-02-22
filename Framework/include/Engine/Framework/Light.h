#pragma once

#include "Engine/Framework/Component.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <Engine/Core/Log/Logger.h>

namespace Engine::Framework
{
	class Entity;
	class Scene;
}

namespace Engine::Framework::Lights
{
	class Light : public Component
	{
	public:
		Light() : Component(), m_Color(1.0f), m_Intensity(1.0f), m_Direction(1.0f) {}
		virtual ~Light() = default;

		virtual void Init() {}

		const glm::vec4& GetColor() const { return m_Color; }
		const void SetColor(const glm::vec4& color) { m_Color = color; }

		float GetIntensity() const { return m_Intensity; }
		void SetIntensity(float intensity) { m_Intensity = intensity; }

		virtual glm::vec3 GetDirection() const { return m_Direction; }
		virtual void SetDirection(const glm::vec3& dir) { m_Direction = dir; }

		virtual int GetShadowType() const { return m_ShadowType; }
		virtual void SetShadowType(int type) { m_ShadowType = type; }

		virtual float GetShadowBias() const { return m_ShadowBias; }
		virtual void SetShadowBias(float bias)
		{
			if (m_ShadowType != 3) // If not PCSS shadows type
				m_ShadowBias = bias;
			else
				m_ShadowBias = glm::clamp(m_ShadowBias, 0.001f, 0.05f);
		}

		virtual float GetShadowStrength() const { return m_ShadowStrength; }
		virtual void SetShadowStrength(float strength) { m_ShadowStrength = strength; }

		virtual int GetShadowResolutionType() const { return m_ShadowResolutionType; }
		virtual void SetShadowResolutionType(int shadowResolutionType) { m_ShadowResolutionType = shadowResolutionType; }

		virtual uint32_t GetShadowResolution() const
		{
			auto res = ConvertResolutionTypeToSize(m_ShadowResolutionType);
			//CRTN_LOG_INFO("Current Shadow Res: %d", res);
			return res;
		}

		virtual float GetPCSSLightSize() const { return m_PCSSLightSize; }
		virtual void SetPCSSLightSize(float pcssLightSize) { m_PCSSLightSize = pcssLightSize; }

	private:
		uint32_t ConvertResolutionTypeToSize(int resolutionType) const
		{
			switch (resolutionType)
			{
			case 0:
				return 256;
				break;
			case 1:
				return 512;
				break;
			case 2:
				return 1024;
				break;
			case 3:
				return 2048;
				break;
			case 4:
				return 4096;
				break;
			case 5:
				return 8192;
				break;
			}
		}
	protected:
		glm::vec4 m_Color;
		float m_Intensity;

		glm::vec3 m_Direction;

		/*
		*  ----- SHADOW TYPES -----
		*  0 - No shadows
		*  1 - Hard Shadows
		*  2 - Soft Shadows (PCF)
		*  3 - Soft Shadows (PCSS)
		*/
		int m_ShadowType = 1;

		float m_ShadowBias = 0.002f;
		float m_ShadowStrength = 0.9f;

		int m_ShadowResolutionType = 1;

		float m_PCSSLightSize = 0.01f;
	};

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight() : Light() {}
		virtual ~DirectionalLight() = default;

		void Init() override;

		static std::unique_ptr<DirectionalLight> Create() { return std::make_unique<DirectionalLight>(); }
	};

	class PointLight : public Light
	{
	public:
		PointLight() : Light() {}
		virtual ~PointLight() = default;

		void Init() override;

		static std::unique_ptr<PointLight> Create() { return std::make_unique<PointLight>(); }
	};
}