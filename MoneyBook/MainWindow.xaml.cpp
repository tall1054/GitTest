#include "pch.h"
#include "MainWindow.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::MoneyBook::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
        DateInput().Date(Windows::Foundation::DateTime::clock::now());
    }

    hstring MainWindow::Currency(double value)
    {
        std::wostringstream out;
        out << L"₩" << std::fixed << std::setprecision(0) << value;
        return hstring{ out.str() };
    }

    void MainWindow::Add_Click(IInspectable const&, RoutedEventArgs const&)
    {
        const double amount = AmountInput().Value();
        const std::wstring memo = MemoInput().Text().c_str();
        if (!std::isfinite(amount) || amount <= 0 || memo.empty())
        {
            return;
        }

        const auto calendar = Windows::Globalization::Calendar{};
        calendar.SetDateTime(DateInput().Date());
        std::wostringstream date;
        date << calendar.Year() << L"-" << std::setfill(L'0') << std::setw(2) << calendar.Month()
             << L"-" << std::setw(2) << calendar.Day();

        m_transactions.push_back({ date.str(), memo, amount, TypeInput().SelectedIndex() == 1 });
        MemoInput().Text(L"");
        AmountInput().Value(std::numeric_limits<double>::quiet_NaN());
        Refresh();
    }

    void MainWindow::Delete_Click(IInspectable const&, RoutedEventArgs const&)
    {
        const int index = TransactionList().SelectedIndex();
        if (index >= 0 && static_cast<size_t>(index) < m_transactions.size())
        {
            m_transactions.erase(m_transactions.begin() + index);
            Refresh();
        }
    }

    void MainWindow::Refresh()
    {
        double income = 0;
        double expense = 0;
        TransactionList().Items().Clear();

        for (auto const& item : m_transactions)
        {
            (item.income ? income : expense) += item.amount;
            std::wostringstream row;
            row << item.date << L"  " << (item.income ? L"[수입] " : L"[지출] ")
                << item.memo << L"  " << Currency(item.amount).c_str();
            TransactionList().Items().Append(box_value(hstring{ row.str() }));
        }

        IncomeText().Text(Currency(income));
        ExpenseText().Text(Currency(expense));
        BalanceText().Text(Currency(income - expense));
    }
}
