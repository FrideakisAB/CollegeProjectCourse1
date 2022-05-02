#include "Game/UI/InfoWindow.h"

#include "imgui.h"
#include "Engine.h"

void InfoWindow::Draw()
{
    auto windowRect = Engine::Get().GetRender().GetWindowSize();

    ImGui::SetNextWindowPos({static_cast<float>(windowRect.x - 430), 0});
    ImGui::SetNextWindowSize({430, 250});
    ImGui::Begin("Управление", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    {
        ImGui::Text("Процесс");
        if (ImGui::BeginTable("Процесс", 2))
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("Esc").x) / 2);
            ImGui::TextColored({200, 30, 0, 255}, "Esc");
            ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("R").x) / 2);
            ImGui::TextColored({200, 30, 0, 255}, "R");
            ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("Пробел").x) / 2);
            ImGui::TextColored({200, 30, 0, 255}, "Пробел");

            ImGui::TableNextColumn();

            ImGui::TextColored({255, 255, 206, 255}, "выход");
            ImGui::TextColored({255, 255, 206, 255}, "повтор запуска");
            ImGui::TextColored({255, 255, 206, 255}, "старт/пауза");

            ImGui::EndTable();
        }

        ImGui::Text("Перемещение камеры");
        if (ImGui::BeginTable("Перемещение камеры", 2))
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("W,A,S,D").x) / 2);
            ImGui::TextColored({200, 30, 0, 255}, "W,A,S,D");
            ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("W,A,S,D").x) / 2);
            ImGui::TextColored({200, 30, 0, 255}, "Стрелки");
            ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("Shift").x) / 2);
            ImGui::TextColored({200, 30, 0, 255}, "Shift");
            ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("Колёсико").x) / 2);
            ImGui::TextColored({200, 30, 0, 255}, "Колёсико");

            ImGui::TableNextColumn();

            ImGui::TextColored({255, 255, 206, 255}, "вверх, влево, вниз, вправо");
            ImGui::TextColored({255, 255, 206, 255}, "вверх, влево, вниз, вправо");
            ImGui::TextColored({255, 255, 206, 255}, "ускорить движение камеры");
            ImGui::TextColored({255, 255, 206, 255}, "приблизить/отдалить");

            ImGui::EndTable();
        }
    }
    ImGui::End();
}
