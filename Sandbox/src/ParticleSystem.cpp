#include "ParticleSystem.h"

#include "Random.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem(uint32_t maxParticles) :
	m_PoolIndex(maxParticles - 1),
	m_WhiteTexture(Engine::Texture2D::Create("assets/textures/1x1.png"))
{
	m_ParticlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(Engine::Timestep timestep)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= timestep;
		particle.Position += particle.Velocity * (float)timestep;
		particle.Rotation += 0.01f * timestep;
	}
}

void ParticleSystem::OnRender(Engine::OrthographicCamera& camera)
{
	Engine::Renderer2D::BeginScene(camera);
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		float sideSize = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
		glm::vec2 size = { sideSize, sideSize };
		glm::vec3 position = { particle.Position.x, particle.Position.y, 1.0f };

		Engine::Renderer2D::DrawRotatedQuad(position, size, particle.Rotation, m_WhiteTexture, 1.0f, color);
	}
	Engine::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProperties& particleProperties)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProperties.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProperties.Velocity;
	particle.Velocity.x += particleProperties.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProperties.VelocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.ColorBegin = particleProperties.ColorBegin;
	particle.ColorEnd = particleProperties.ColorEnd;

	particle.LifeTime = particleProperties.LifeTime;
	particle.LifeRemaining = particleProperties.LifeTime;
	particle.SizeBegin = particleProperties.SizeBegin + particleProperties.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProperties.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}