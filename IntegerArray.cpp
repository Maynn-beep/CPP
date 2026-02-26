#include "IntegerArray.h"

#include <algorithm>
#include <cstddef>

// bad_length

const char* bad_length::what() const noexcept
{
    return "Invalid IntegerArray length";
}

// bad_range

const char* bad_range::what() const noexcept
{
    return "IntegerArray index out of range";
}

// IntegerArray

IntegerArray::IntegerArray(int length)
    : m_length{ length }
    , m_data{ nullptr }
{
    if (length < 0)
    {
        throw bad_length{};
    }

    if (length > 0)
    {
        m_data = new int[static_cast<std::size_t>(length)]{};
    }
}

IntegerArray::~IntegerArray()
{
    delete[] m_data;
}

IntegerArray::IntegerArray(const IntegerArray& other)
    : IntegerArray(other.m_length)
{
    if (m_length > 0)
    {
        std::copy_n(other.m_data, static_cast<std::size_t>(m_length), m_data);
    }
}

IntegerArray& IntegerArray::operator=(const IntegerArray& other)
{
    if (this == &other)
    {
        return *this;
    }

    reallocate(other.m_length);

    if (m_length > 0)
    {
        std::copy_n(other.m_data, static_cast<std::size_t>(m_length), m_data);
    }

    return *this;
}

void IntegerArray::erase()
{
    delete[] m_data;
    m_data = nullptr;
    m_length = 0;
}

void IntegerArray::reallocate(int newLength)
{
    if (newLength < 0)
    {
        throw bad_length{};
    }

    erase();

    if (newLength == 0)
    {
        return;
    }

    m_data = new int[static_cast<std::size_t>(newLength)]{};
    m_length = newLength;
}

void IntegerArray::resize(int newLength)
{
    if (newLength == m_length)
    {
        return;
    }

    if (newLength < 0)
    {
        throw bad_length{};
    }

    if (newLength == 0)
    {
        erase();
        return;
    }

    int* data{ new int[static_cast<std::size_t>(newLength)]{} };

    if (m_length > 0)
    {
        const int elementsToCopy{ (newLength > m_length) ? m_length : newLength };
        std::copy_n(m_data, static_cast<std::size_t>(elementsToCopy), data);
    }

    delete[] m_data;
    m_data = data;
    m_length = newLength;
}

int& IntegerArray::operator[](int index)
{
    if (index < 0 || index >= m_length)
    {
        throw bad_range{};
    }

    return m_data[index];
}

const int& IntegerArray::operator[](int index) const
{
    if (index < 0 || index >= m_length)
    {
        throw bad_range{};
    }

    return m_data[index];
}

void IntegerArray::insertBefore(int value, int index)
{
    if (index < 0 || index > m_length)
    {
        throw bad_range{};
    }

    int* data{ new int[static_cast<std::size_t>(m_length + 1)] };

    // copy elements before index
    if (index > 0 && m_length > 0)
    {
        std::copy_n(m_data, static_cast<std::size_t>(index), data);
    }

    // insert new element
    data[index] = value;

    // copy elements after index
    if (index < m_length)
    {
        std::copy_n(
            m_data + index,
            static_cast<std::size_t>(m_length - index),
            data + index + 1
        );
    }

    delete[] m_data;
    m_data = data;
    ++m_length;
}

void IntegerArray::remove(int index)
{
    if (index < 0 || index >= m_length)
    {
        throw bad_range{};
    }

    if (m_length == 1)
    {
        erase();
        return;
    }

    int* data{ new int[static_cast<std::size_t>(m_length - 1)] };

    // copy elements before index
    if (index > 0)
    {
        std::copy_n(m_data, static_cast<std::size_t>(index), data);
    }

    // copy elements after index
    if (index < m_length - 1)
    {
        std::copy_n(
            m_data + index + 1,
            static_cast<std::size_t>(m_length - index - 1),
            data + index
        );
    }

    delete[] m_data;
    m_data = data;
    --m_length;
}

void IntegerArray::insertAtBeginning(int value)
{
    insertBefore(value, 0);
}

void IntegerArray::insertAtEnd(int value)
{
    insertBefore(value, m_length);
}

int IntegerArray::getLength() const
{
    return m_length;
}

