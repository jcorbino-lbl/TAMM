#ifndef TAMM_TENSOR_HPP_
#define TAMM_TENSOR_HPP_

#include "tamm/tensor_impl.hpp"

namespace tamm {

/**
 * @ingroup tensors
 * @brief Templated Tensor class designed using PIMPL (pointer to
 * implementation) idiom. All of the implementation (except the static
 * methods) are done in TensorImpl<T> class
 *
 * @tparam T type for the Tensor value
 */
template<typename T>
class Tensor {
public:
    /**
     * @brief Construct a scalar Tensor with 0-modes
     *
     */
    Tensor() : impl_{std::make_shared<TensorImpl<T>>()} {}

    /**
     * @brief Construct a new Tensor object from a vector of TiledIndexSpace
     * objects
     *
     * @param [in] tis_vec a vector of TiledIndexSpace objects for each mode
     */
    Tensor(std::vector<TiledIndexSpace> tis_vec) :
      impl_{std::make_shared<TensorImpl<T>>(tis_vec)} {}

    /**
     * @brief Construct a new Tensor object from a vector of TiledIndexLabel
     * objects
     *
     * @param [in] til_vec a vector of TiledIndexLabel objects which will be
     * used to extract TiledIndexSpace for Tensor construction
     */
    Tensor(std::vector<TiledIndexLabel> til_vec) :
      impl_{std::make_shared<TensorImpl<T>>(til_vec)} {}

    // SpinTensor Constructors
    /**
     * @brief Construct a new Tensor object with Spin attributes
     *
     * @param [in] t_spaces vector of TiledIndexSpace objects for each mode
     * @param [in] spin_mask spin mask for each mode
     */
    Tensor(TiledIndexSpaceVec t_spaces, SpinMask spin_mask) :
      impl_{std::make_shared<TensorImpl<T>>(t_spaces, spin_mask)} {}

    /**
     * @brief Construct a new Tensor object with Spin attributes
     *
     * @param [in] t_spaces initializer list of TiledIndexSpace objects for each
     * mode
     * @param [in] spin_mask spin mask for each mode
     */
    Tensor(std::initializer_list<TiledIndexSpace> t_spaces,
           SpinMask spin_mask) :
      impl_{std::make_shared<TensorImpl<T>>(t_spaces, spin_mask)} {}

    /**
     * @brief Construct a new Tensor object with Spin attributes
     *
     * @param [in] t_lbls vector of TiledIndexLabel objects for each mode
     * @param [in] spin_mask spin mask for each mode
     */
    Tensor(IndexLabelVec t_lbls, SpinMask spin_mask) :
      impl_{std::make_shared<TensorImpl<T>>(t_lbls, spin_mask)} {}

    /**
     * @brief Construct a new Tensor object with Spin attributes
     *
     * @param [in] t_spaces vector of TiledIndexSpace objects for each mode
     * @param [in] spin_sizes vector of sizes for each spin attribute
     */
    Tensor(TiledIndexSpaceVec t_spaces, std::vector<size_t> spin_sizes) :
      impl_{std::make_shared<TensorImpl<T>>(t_spaces, spin_sizes)} {}

    /**
     * @brief Construct a new Tensor object with Spin attributes
     *
     * @param [in] t_spaces initializer list of TiledIndexSpace objects for each
     * mode
     * @param [in] spin_sizes vector of sizes for each spin attribute
     */
    Tensor(std::initializer_list<TiledIndexSpace> t_spaces,
           std::vector<size_t> spin_sizes) :
      impl_{std::make_shared<TensorImpl<T>>(t_spaces, spin_sizes)} {}

    /**
     * @brief Construct a new Tensor object with Spin attributes
     *
     * @param [in] t_spaces vector of TiledIndexLabel objects for each mode
     * @param [in] spin_sizes vector of sizes for each spin attribute
     */
    Tensor(IndexLabelVec t_lbls, std::vector<size_t> spin_sizes) :
      impl_{std::make_shared<TensorImpl<T>>(t_lbls, spin_sizes)} {}

    // LambdaTensor Constructors
    /**
     * @brief Signature description for Lambda functor
     *
     */
    using Func = std::function<void(const IndexVector&, span<T>)>;

    /**
     * @brief Construct a new Tensor object with a Lambda function
     *
     * @param [in] t_spaces vector of TiledIndexSpace objects for each mode
     * @param [in] lambda Lambda function for Tensor construction
     * @warning Tensor objects constructed using Lambda function is a read-only
     * Tensor. It can only be on right-hand side of an equation and @param
     * lambda will be called as a get access on the Tensor.
     */
    Tensor(TiledIndexSpaceVec t_spaces, Func lambda) :
      impl_{std::make_shared<LambdaTensorImpl<T>>(t_spaces, lambda)} {}

    /**
     * @brief Construct a new Tensor object with a Lambda function
     *
     * @param [in] t_spaces initializer list  of TiledIndexSpace objects for
     * each mode
     * @param [in] lambda Lambda function for Tensor construction
     * @warning Tensor objects constructed using Lambda function is a read-only
     * Tensor. It can only be on right-hand side of an equation and @param
     * lambda will be called as a get access on the Tensor.
     */
    Tensor(std::initializer_list<TiledIndexSpace> t_spaces, Func lambda) :
      impl_{std::make_shared<LambdaTensorImpl<T>>(t_spaces, lambda)} {}

    /**
     * @brief Construct a new Tensor object from a set of TiledIndexSpace
     * objects as modes of the Tensor
     *
     * @param [in] tis set of TiledIndexSpace objects for each mode
     */
    Tensor(std::initializer_list<TiledIndexSpace> tis) :
      impl_{std::make_shared<TensorImpl<T>>(tis)} {}

    /**
     * @brief Construct a new Tensor object from a set of TiledIndexLabel
     * objects that are used to extract TiledIndexSpace information as
     * modes of the Tensor
     *
     * @param [in] tis set of TiledIndexLabel objects for each mode
     */
    Tensor(const std::initializer_list<TiledIndexLabel>& lbls) :
      impl_{std::make_shared<TensorImpl<T>>(lbls)} {}

    /**
     * @brief Constructs a new Tensor object recursively with a set of
     * TiledIndexSpace objects followed by a lambda expression
     *
     * @tparam Ts variadic template for the input arguments
     * @param [in] tis TiledIndexSpace object for the corresponding mode of
     * the Tensor object
     * @param [in] rest remaining parts of the input arguments
     */
    template<class... Ts>
    Tensor(const TiledIndexSpace& tis, Ts... rest) :
      impl_{std::make_shared<TensorImpl<T>>(tis, rest...)} {}

    /**
     * @brief Operator overload for constructing a LabeledTensor object with
     * input TiledIndexLabel objects (main construct for Tensor operations)
     *
     * @tparam Ts variadic template for set of input TiledIndexLabels
     * @param [in] input TiledIndexLabels
     * @returns a LabeledTensor object created with the input arguments
     */

    template<class... Args>
    LabeledTensor<T> operator()(Args&&... rest) const {
        return LabeledTensor<T>{*this, std::forward<Args>(rest)...};
    }

    // Tensor Accessors
    /**
     * @brief Get method for Tensor values
     *
     * @param [in] idx_vec set of indices to get data from
     * @param [in] buff_span a memory span to write the fetched data
     */
    void get(IndexVector idx_vec, span<T> buff_span) const {
        impl_->get(idx_vec, buff_span);
    }

    /**
     * @brief Put method for Tensor values
     *
     * @param [in] idx_vec set of indices to put data to
     * @param [in] buff_span a memory span for the data to be put
     */
    void put(IndexVector idx_vec, span<T> buff_span) {
        impl_->put(idx_vec, buff_span);
    }

    /**
     * @brief Add method for Tensor values
     *
     * @param [in] idx_vec set of indices to put data to
     * @param [in] buff_span a memory span for the data to be put
     */
    void add(IndexVector idx_vec, span<T> buff_span) {
        impl_->add(idx_vec, buff_span);
    }

    /**
     * @brief Constructs a LabeledLoopNest object  for Tensor object
     *
     * @returns a LabelLoopNest for the Tensor
     */
    LabelLoopNest loop_nest() const { return impl_->loop_nest(); }

    /**
     * @brief Gets the diagonal sum of the Tensor object
     *
     * @returns sum of the diagonal values
     * @warning available for tensors with 2 modes
     */
    T trace() const { return impl_->trace(); }

    /**
     * @brief Gets the diagonal values from the Tensor object
     *
     * @returns a vector of values from the diagonal of Tensor
     * @warning available for tensors with 2 modes
     */
    std::vector<T> diagonal() const { return impl_->diagonal(); }

    /**
     * @brief Get the size of a block
     *
     * @param [in] blockid The id of the block
     * @return size_t The size of the block
     */
    size_t block_size(const IndexVector& blockid) const {
        return impl_->block_size(blockid);
    }

    /**
     * @brief Get dimensions of a block
     *
     * @param [in] blockid The id of the block
     * @return std::vector<size_t>  The vector of dimensions
     */
    std::vector<size_t> block_dims(const IndexVector& blockid) const {
        return impl_->block_dims(blockid);
    }

    /**
     * @brief Get offsets of a block
     *
     * @param [in] blockid The id of the block
     * @returns std::vector<size_t> The vector of offsets
     */
    std::vector<size_t> block_offsets(const IndexVector& blockid) const {
        return impl_->block_offsets(blockid);
    }

    /**
     * @brief Get index spaces of a vector
     *
     * @return const std::vector<TiledIndexSpace>&
     */
    const std::vector<TiledIndexSpace>& tiled_index_spaces() const {
        return impl_->tiled_index_spaces();
    }

    /**
     * @brief Return dependency map of the tensor's index spaces
     *
     * @return const std::map<size_t,std::vector<size_t>>& The dependence map
     * that maps indices of index spaces to a vector of indices that each space
     * depends on.
     */
    const std::map<size_t, std::vector<size_t>>& dep_map() const {
        return impl_->dep_map();
    }

    /**
     * @brief Memory allocation method for the Tensor object
     *
     */
    void allocate(ExecutionContext* ec) { impl_->allocate(ec); }

    /**
     * @brief Memory deallocation method for the Tensor object
     *
     */
    void deallocate() { impl_->deallocate(); }

    // Static methods for allocate/deallocate
    /**
     * @brief Static memory allocation method for a set of Tensors
     *
     * @tparam Args variadic template for set of Tensor objects
     * @param [in] ec input ExecutionContext object to be used for
     * allocation
     * @param [in] rest set of Tensor objects to be allocated
     */
    template<typename... Args>
    static void allocate(ExecutionContext* ec, Tensor<T>& tensor,
                         Args&... rest) {
        alloc(ec, tensor, rest...);
    }

    /**
     * @brief Static memory deallocation method for a set of Tensors
     *
     * @tparam Args variadic template for set of Tensor objects
     * @param [in] rest set of Tensor objects to be deallocated
     */
    template<typename... Args>
    static void deallocate(Tensor<T>& tensor, Args&... rest) {
        dealloc(tensor, rest...);
    }

    /**
     * @brief Get the number of modes of a Tensor
     *
     * @returns number of modes of a Tensor
     */
    size_t num_modes() const { return impl_->num_modes(); }

    /**
     * @brief Checks if the block is non-zero (calculated using Spin attributes)
     *
     * @param [in] blockid identifier for the block
     * @returns true if the block is non-zero
     */
    bool is_non_zero(const IndexVector& blockid) const {
        return impl_->is_non_zero(blockid);
    }

private:
    std::shared_ptr<TensorImpl<T>>
      impl_; /**< Shared pointer to the implementation object */

    // Private allocate and de-allocate functions
    /**
     * @brief Static allocation method (used internally only)
     *
     * @param [in] ec ExecutionContext for the allocation
     */
    static void alloc(const ExecutionContext* ec) {}

    /**
     * @brief Static deallocation method (used internally only)
     *
     */
    static void dealloc() {}

    /**
     * @brief Static memory allocation method for a set of Tensors
     *
     * @tparam Args variadic template for set of Tensor objects
     * @param [in] ec input ExecutionContext object to be used for
     * allocation
     * @param [in] rest set of Tensor objects to be allocated
     */
    template<typename... Args>
    static void alloc(ExecutionContext* ec, Tensor<T>& tensor, Args&... rest) {
        tensor.impl_->allocate(ec);
        alloc(ec, rest...);
    }

    /**
     * @brief Static memory deallocation method for a set of Tensors
     *
     * @tparam Args variadic template for set of Tensor objects
     * @param [in] rest set of Tensor objects to be deallocated
     */
    template<typename... Args>
    static void dealloc(Tensor<T>& tensor, Args&... rest) {
        tensor.impl_->deallocate();
        dealloc(rest...);
    }
};

} // namespace tamm

#endif // TENSOR_HPP_
