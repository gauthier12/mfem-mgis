/*!
 * \file   src/FiniteElementDiscretization.cxx
 * \brief
 * \author Thomas Helfer
 * \date 16/12/2020
 */

#include <utility>
#include <mfem/fem/fespace.hpp>
#ifdef MFEM_USE_MPI
#include <mfem/fem/pfespace.hpp>
#endif
#include "MGIS/Raise.hxx"
#include "MFEMMGIS/FiniteElementDiscretization.hxx"

namespace mfem_mgis {

  void FiniteElementDiscretization::reportInvalidParallelMesh() {
    mgis::raise(
        "FiniteElementDiscretization::reportInvalidParallelMesh: "
        "no parallel mesh defined");
  }  // end of reportInvalidParallelMesh

  void FiniteElementDiscretization::reportInvalidSequentialMesh() {
    mgis::raise(
        "FiniteElementDiscretization::reportInvalidSequentialMesh: "
        "no sequential mesh defined");
  }  // end of reportInvalidSequentialMesh

  void FiniteElementDiscretization::reportInvalidParallelFiniteElementSpace() {
    mgis::raise(
        "FiniteElementDiscretization::reportInvalidParallelFiniteElementSpace: "
        "no parallel finite element space defined");
  }  // end of reportInvalidParallelFiniteElementSpace

  void
  FiniteElementDiscretization::reportInvalidSequentialFiniteElementSpace() {
    mgis::raise(
        "FiniteElementDiscretization::"
        "reportInvalidSequentialFiniteElementSpace: "
        "no sequential finite element space defined");
  }  // end of reportInvalidSequentialFiniteElementSpace

#ifdef MFEM_USE_MPI
  FiniteElementDiscretization::FiniteElementDiscretization(
      std::shared_ptr<Mesh<true>> m,
      std::shared_ptr<const FiniteElementCollection> c,
      const size_type d)
      : parallel_mesh(std::move(m)), fec(std::move(c)) {
    this->parallel_fe_space = std::make_unique<FiniteElementSpace<true>>(
        this->parallel_mesh.get(), this->fec.get(), d);
  }    // end of FiniteElementDiscretization
#else  /* MFEM_USE_MPI */
  FiniteElementDiscretization::FiniteElementDiscretization(
      std::shared_ptr<Mesh<true>>,
      std::shared_ptr<const FiniteElementCollection>,
      const size_type) {
    reportUnsupportedParallelComputations();
  }  // end of FiniteElementDiscretization
#endif /* MFEM_USE_MPI */

#ifdef MFEM_USE_MPI
  FiniteElementDiscretization::FiniteElementDiscretization(
      std::shared_ptr<Mesh<true>> m,
      std::shared_ptr<const FiniteElementCollection> c,
      std::unique_ptr<FiniteElementSpace<true>> s)
      : parallel_mesh(std::move(m)),
        fec(std::move(c)),
        parallel_fe_space(std::move(s)) {
    if (this->parallel_mesh.get() != this->parallel_fe_space->GetMesh()) {
      mgis::raise(
          "FiniteElementDiscretization::FiniteElementDiscretization: "
          "mesh pointer don't match the mesh on which the finite element space "
          "is built");
    }
  }    // end of FiniteElementDiscretization
#else  /* MFEM_USE_MPI */
  FiniteElementDiscretization::FiniteElementDiscretization(
      std::shared_ptr<Mesh<true>>,
      std::shared_ptr<const FiniteElementCollection>,
      std::unique_ptr<FiniteElementSpace<true>>) {
    reportUnsupportedParallelComputations();
  }  // end of FiniteElementDiscretization
#endif /* MFEM_USE_MPI */

  FiniteElementDiscretization::FiniteElementDiscretization(
      std::shared_ptr<Mesh<false>> m,
      std::shared_ptr<const FiniteElementCollection> c,
      const size_type d)
      : sequential_mesh(std::move(m)), fec(std::move(c)) {
    this->sequential_fe_space = std::make_unique<FiniteElementSpace<false>>(
        this->sequential_mesh.get(), this->fec.get(), d);
  }  // end of FiniteElementDiscretization

  FiniteElementDiscretization::FiniteElementDiscretization(
      std::shared_ptr<Mesh<false>> m,
      std::shared_ptr<const FiniteElementCollection> c,
      std::unique_ptr<FiniteElementSpace<false>> s)
      : sequential_mesh(std::move(m)),
        fec(std::move(c)),
        sequential_fe_space(std::move(s)) {
    if (this->sequential_mesh.get() != this->sequential_fe_space->GetMesh()) {
      mgis::raise(
          "FiniteElementDiscretization::FiniteElementDiscretization: "
          "mesh pointer don't match the mesh on which the finite element space "
          "is built");
    }
  }  // end of FiniteElementDiscretization

  bool FiniteElementDiscretization::describesAParallelComputation() const {
#ifdef MFEM_USE_MPI
    return this->parallel_mesh.get() != nullptr;
#else  /* MFEM_USE_MPI */
    return false;
#endif /* MFEM_USE_MPI */
  }    // end of describesAParallelComputation

  const FiniteElementCollection&
  FiniteElementDiscretization::getFiniteElementCollection() const {
    return *(this->fec);
  }  // end of getFiniteElementCollection

  FiniteElementDiscretization::~FiniteElementDiscretization() = default;

  size_type getTrueVSize(const FiniteElementDiscretization& fed) {
    if (fed.describesAParallelComputation()) {
#ifdef MFEM_USE_MPI
      return fed.getFiniteElementSpace<true>().GetTrueVSize();
#else  /* MFEM_USE_MPI */
      reportUnsupportedParallelComputations();
#endif /* MFEM_USE_MPI */
    }
    return fed.getFiniteElementSpace<false>().GetTrueVSize();
  }  // end of getTrueVSize

}  // end of namespace mfem_mgis
