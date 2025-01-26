/**************************************************************************/
/* smart_ptr.hpp                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                                Knoodle                                 */
/*                        https://knoodlegraph.org                        */
/**************************************************************************/
/* Copyright (c) 2025 Knoodle contributors (vide AUTHORS.md)              */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

namespace kn {
inline namespace internal {
template <typename T>
class SmartPtrRef {
 public:
  SmartPtrRef(T* ptr) throw() : _ptr(ptr) {}

  operator void**() const throw() { return reinterpret_cast<void**>(_ptr->release_and_get_address_of()); }

  operator T*() throw() {
    *_ptr = nullptr;
    return _ptr;
  }

  operator T**() throw() { return _ptr->release_and_get_address_of(); }

  T* operator*() throw() { return _ptr->get(); }

  T* const* get_address_of() const throw() { return _ptr->get_address_of(); }

  T** release_and_get_address_of() throw() { return _ptr->release_and_get_address_of(); }

  operator IUnknown**() const throw() {
    static_assert(std::is_base_of_v<IUnknown, T>);
    return reinterpret_cast<IUnknown**>(_ptr->ReleaseAndGetAddressOf());
  }

 private:
  T* _ptr;
};
}  // namespace internal

template <typename T>
class SmartPtr {
 public:
  SmartPtr() throw() : _ptr(nullptr) {}

  SmartPtr(std::nullptr_t) throw() : _ptr(nullptr) {}

  template <typename U>
  SmartPtr(U* other) throw() : _ptr(other) {
    internalAddRef();
  }

  SmartPtr(const SmartPtr& other) throw() : _ptr(other._ptr) { internalAddRef(); }

  template <typename U>
  SmartPtr(const SmartPtr<U>& other) throw() : _ptr(other.get()) {
    internalAddRef();
  }

  SmartPtr(SmartPtr&& other) throw() : _ptr(nullptr) {
    if (this != reinterpret_cast<SmartPtr*>(&reinterpret_cast<uint8_t&>(other))) {
      other.swap(*this);
    }
  }

  template <typename U>
  SmartPtr(SmartPtr<U>&& other, typename std::enable_if<std::is_convertible<U*, T*>::value>::type* = nullptr) throw()
      : _ptr(other._ptr) {
    other._ptr = nullptr;
  }

  ~SmartPtr() throw() { internalRelease(); }

  SmartPtr& operator=(std::nullptr_t) throw() {
    internalRelease();
    return *this;
  }

  SmartPtr& operator=(T* other) throw() {
    if (_ptr != other) {
      SmartPtr(other).swap(*this);
    }
    return *this;
  }

  template <typename U>
  SmartPtr& operator=(U* other) throw() {
    SmartPtr(other).swap(*this);
    return *this;
  }

  SmartPtr& operator=(const SmartPtr& other) throw() {
    if (_ptr != other._ptr) {
      SmartPtr(other).swap(*this);
    }
    return *this;
  }

  template <typename U>
  SmartPtr& operator=(const SmartPtr<U>& other) throw() {
    SmartPtr(other).swap(*this);
    return *this;
  }

  SmartPtr& operator=(SmartPtr&& other) throw() {
    SmartPtr(static_cast<SmartPtr&&>(other)).swap(*this);
    return *this;
  }

  template <typename U>
  SmartPtr& operator=(SmartPtr<U>&& other) throw() {
    SmartPtr(static_cast<SmartPtr<U>&&>(other)).swap(*this);
    return *this;
  }

  void swap(SmartPtr&& r) throw() {
    T* tmp = _ptr;
    _ptr = r._ptr;
    r._ptr = tmp;
  }

  void swap(SmartPtr& r) throw() {
    T* tmp = _ptr;
    _ptr = r._ptr;
    r._ptr = tmp;
  }

  operator bool() const throw() { return get() != nullptr; }

  T* get() const throw() { return _ptr; }

  T* operator->() const throw() { return _ptr; }

  internal::SmartPtrRef<SmartPtr<T>> operator&() throw() { return internal::SmartPtrRef<SmartPtr<T>>(this); }

  const internal::SmartPtrRef<const SmartPtr<T>> operator&() const throw() {
    return internal::SmartPtrRef<const SmartPtr<T>>(this);
  }

  T* const* get_address_of() const throw() { return &_ptr; }

  T** get_address_of() throw() { return &_ptr; }

  T** release_and_get_address_of() throw() {
    internalRelease();
    return &_ptr;
  }

  T* detach() throw() {
    T* ptr = _ptr;
    _ptr = nullptr;
    return ptr;
  }

  void attach(_In_opt_ T* other) throw() {
    if (_ptr != nullptr) {
      [[maybe_unused]] uint32_t ref = _ptr->RefRelease();
      assert(ref != 0 || _ptr != other);
    }

    _ptr = other;
  }

  uint32_t reset() { return internalRelease(); }

 protected:
  void internalAddRef() const throw() {
    if (_ptr != nullptr) {
      _ptr->addRef();
    }
  }

  uint32_t internalRelease() throw() {
    uint32_t ref = 0;
    T* temp = _ptr;

    if (temp != nullptr) {
      _ptr = nullptr;
      ref = temp->release();
    }

    return ref;
  }

  T* _ptr;
};
}  // namespace kn
