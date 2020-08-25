#pragma once

#include "Application/ImGuiLab.h"
#include "Core/HashMap.h"
#include "Utils/Profiler.h"

class ProfileGraph
{
public:
    using Session = Profiler::SessionData;

    int32 frameWidth = 3;
    int32 frameSpacing = 1;

    ProfileGraph(int32 frameCount = 300)
    {
        frameDatas.SetCount(frameCount);
    }

    void AddFrameData(const Array<Session> &sessions)
    {
        auto &frame = frameDatas[currentFrameIndex];

        frame.sessions.Clear();
        for (int32 i = 0; i < sessions.Count(); ++i)
        {
            frame.sessions.Add(sessions[i]);
        }

        frame.statsIndices.SetCount(frame.sessions.Count());
        for (int32 i = 0; i < frame.sessions.Count(); ++i)
        {
            auto &s = frame.sessions[i];
            if (!nameToStatsIndex.Contains(s.name))
            {
                nameToStatsIndex.Put(s.name, statsDatas.Count());
                statsDatas.Add({});
            }
            frame.statsIndices[i] = nameToStatsIndex[s.name];
        }

        currentFrameIndex = (currentFrameIndex + 1) % frameDatas.Count();
        RebuildStats(currentFrameIndex, frameDatas.Count());
    }

    void Render(int32 graphWidth, int32 legendWidth, int32 height, int32 frameIndexOffset)
    {
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        auto pos = ImGui::GetCursorScreenPos();
        RenderGraph(drawList, Vector2(pos.x, pos.y), Vector2(graphWidth, height), frameIndexOffset);
        RenderLegend(drawList, Vector2(pos.x + graphWidth, pos.y), Vector2(legendWidth, height), frameIndexOffset);
        ImGui::Dummy(ImVec2(graphWidth + legendWidth, height));
    }

private:
    void RebuildStats(int32 endFrame, int32 frameCount)
    {
        maxFrameTime = 0.0f;

        for (auto &e : statsDatas)
        {
            e.maxTime = -1.0f;
            e.order = -1;
        }

        for (int32 f = 0; f < frameCount; ++f)
        {
            int32 frameIndex = (endFrame - 1 - f + frameCount) % frameCount;
            auto &frame = frameDatas[frameIndex];
            float frameTime = 0.0f;
            for (int32 i = 0; i < frame.sessions.Count(); ++i)
            {
                auto &s = frame.sessions[i];
                auto &stats = statsDatas[frame.statsIndices[i]];
                stats.maxTime = Math::Max(stats.maxTime, s.elapsedMs);
                frameTime += s.elapsedMs;
            }
            frame.time = frameTime;
            maxFrameTime = Math::Max(maxFrameTime, frameTime);
        }

        Array<int32> priorities;
        priorities.SetCount(statsDatas.Count());
        for (int32 i = 0; i < statsDatas.Count(); ++i)
            priorities[i] = i;

        priorities.Sort([this](int32 a, int32 b) {
            return statsDatas[a].maxTime > statsDatas[b].maxTime;
        });

        for (int32 i = 0; i < statsDatas.Count(); ++i)
            statsDatas[priorities[i]].order = i;
    }

    uint32 GetColor(const String &name)
    {
        auto ptr = nameToColor.TryGet(name);
        if (ptr)
            return *ptr;

        auto vec4 = ImGui::ColorConvertU32ToFloat4(Hash::HashValue(name));
        vec4.w = 1.0f;
        uint32 col = ImGui::ColorConvertFloat4ToU32(vec4);
        nameToColor.Put(name, col);
        return col;
    }

    void RenderGraph(ImDrawList *drawList, const Vector2 &graphPos, const Vector2 &graphSize, int32 frameIndexOffset)
    {
        drawList->AddRect(ImVec2(graphPos.x, graphPos.y), ImVec2(graphPos.x + graphSize.x, graphPos.y + graphSize.y), 0xffffffff);
        if (maxFrameTime <= 0.0f)
            return;

        constexpr float stdFrameTime = 1000.0f / 60.f;
        float maxTime = Math::Max(maxFrameTime, 1.2f * stdFrameTime);
        float deltaHeight = (graphSize.y - 1.0f) / maxTime;
        float lineOffsetY = (1.0f - stdFrameTime / maxTime) * (graphSize.y - 1.0f);
        const int32 frameCount = frameDatas.Count();
        for (int32 f = 0; f < frameCount; ++f)
        {
            int32 frameIndex = (currentFrameIndex - frameIndexOffset - 1 - f + 2 * frameCount) % frameCount;
            Vector2 framePos = graphPos + Vector2(graphSize.x - 1.0f - frameWidth - (frameWidth + frameSpacing) * f, graphSize.y - 1.0f);

            if (framePos.x < graphPos.x + 1)
                break;

            Vector2 elePos = framePos;
            for (auto &e : frameDatas[frameIndex].sessions)
            {
                float h = e.elapsedMs * deltaHeight;
                drawList->AddRect(ImVec2(elePos.x, elePos.y), ImVec2(elePos.x + frameWidth, elePos.y - h), GetColor(e.name));
                elePos.y -= h;
            }
        }

        drawList->AddLine(ImVec2(graphPos.x, graphPos.y + lineOffsetY), ImVec2(graphPos.x + graphSize.x, graphPos.y + lineOffsetY), 0xffffffff);
    }

    void RenderLegend(ImDrawList *drawList, const Vector2 &legendPos, const Vector2 &legendSize, int32 frameIndexOffset)
    {
        float rectWidth = 30.0f;
        float rectMargin = 3.0f;
        float rectHeight = 10.0f;
        float rectSpacing = 4.0f;
        float nameOffset = 70.0f;
        float textMargin = 5.0f;

        const int32 frameCount = frameDatas.Count();
        const int32 statsCount = statsDatas.Count();
        int32 frameIndex = (currentFrameIndex - frameIndexOffset - 1 + 2 * frameCount) % frameCount;
        auto &frame = frameDatas[frameIndex];
        int32 maxElementCount = legendSize.y / (rectHeight + rectSpacing);
        int32 showElementCount = Math::Min(maxElementCount, statsCount);

        int32 showOrder = 0;
        for (int32 i = 0; i < frame.sessions.Count(); ++i)
        {
            auto &e = frame.sessions[i];
            auto &stats = statsDatas[frame.statsIndices[i]];
            uint32 color = GetColor(e.name);

            if (stats.order >= showElementCount)
                continue;

            Vector2 rectMin = legendPos + Vector2(rectMargin, legendSize.y - rectMargin - (rectHeight + rectSpacing) * showOrder);
            Vector2 rectMax = rectMin + Vector2(rectWidth, -rectHeight);
            drawList->AddRectFilled(ImVec2(rectMin.x, rectMin.y), ImVec2(rectMax.x, rectMax.y), color);

            int32 microSec = (int32)(e.elapsedMs * 1000);
            int32 iv = (int32)(microSec / 1000);
            int32 fv = fv = (int32)((microSec - iv * 1000) / 10);
            String timeFormat = iv < 10 ? CT_TEXT("[0{0}.") : CT_TEXT("[{0}.");
            timeFormat += fv < 10 ? CT_TEXT("0{1} ms]") : CT_TEXT("{1} ms]");
            String timeStr = String::Format(timeFormat, iv, fv);

            drawList->AddText(ImVec2(rectMax.x + textMargin, rectMax.y), color, CT_U8_CSTR(timeStr));
            drawList->AddText(ImVec2(rectMax.x + textMargin + nameOffset, rectMax.y), color, CT_U8_CSTR(e.name));

            showOrder++;
        }
    }

private:
    struct StatsData
    {
        float maxTime = -1.0f;
        int32 order;
    };

    struct FrameData
    {
        Array<Session> sessions;
        Array<int32> statsIndices;
        float time;
    };

    Array<FrameData> frameDatas;
    int32 currentFrameIndex = 0;
    Array<StatsData> statsDatas;
    HashMap<String, int32> nameToStatsIndex;
    HashMap<String, uint32> nameToColor;
    float maxFrameTime;
};

class ProfileWindow
{
public:
    ProfileGraph graph;
    int32 frameOffset = 0;
    bool open = true;

    void OnGui()
    {
        if (ImGui::Begin("Profiler", &open, ImGuiWindowFlags_NoScrollbar))
        {
            ImVec2 canvasSize = ImGui::GetContentRegionAvail();

            int32 sizeMargin = (int32)ImGui::GetStyle().ItemSpacing.y;
            int32 maxGraphHeight = 300;
            int32 availableGraphHeight = (int32)canvasSize.y - sizeMargin;
            int32 graphHeight = Math::Min(maxGraphHeight, availableGraphHeight);
            int32 legendWidth = 260;
            int32 graphWidth = (int32)canvasSize.x - legendWidth;
            graph.Render(graphWidth, legendWidth, graphHeight, frameOffset);
        }

        ImGui::End();
    }

    void AddFrameData(const Array<ProfileGraph::Session> &sessions)
    {
        graph.AddFrameData(sessions);
    }
};