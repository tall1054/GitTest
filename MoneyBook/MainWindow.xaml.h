#pragma once
#include "MainWindow.xaml.g.h"

namespace winrt::MoneyBook::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
        void Add_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        void Delete_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);

    private:
        struct Transaction { std::wstring date; std::wstring memo; double amount; bool income; };
        std::vector<Transaction> m_transactions;
        void Refresh();
        static hstring Currency(double value);
    };
}
