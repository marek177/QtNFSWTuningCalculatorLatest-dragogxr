# NFSWPerformanceTool algorithm integration

This update ports the newer NFSWPerformanceTool calculation model into QtNFSWTuningCalculatorLatest while preserving the original Qt UI and part database.

## Included changes

- 12-value car model: 4 TopSpeed + 4 Acceleration + 4 Handling values per car.
- Sequential float32 accumulation of the six installed part H/A/T values.
- Recovered normalization constant: `0.6666666865348816f`.
- Final stock/Handling/Acceleration/TopSpeed endpoint blending.
- Display conversion truncates toward zero.
- Tuner uses the new common `GameMath` path.
- Analyzer/Finder uses aggregate H/A/T candidate search followed by exact six-part verification.
- Maximizer uses aggregate H/A/T states instead of the legacy component-wise Pareto assumption for 12-value cars.
- Compare and tuning diagrams use the same new calculation.
- 142 car IDs contain recovered 12-value data.

## Cars using legacy fallback

The supplied 12-value dataset did not contain matching records for:

- GXR
- Porsche Boxster Spyder

Those cars keep the original Qt formula.

## Repository layout

The original repository stores its C++ source inside `QtNFSWTuningCalculatorLatest.rar` rather than as normal `.cpp/.h` files. For that reason this update is stored as a set of unified patches under:

`updates/NFSWPerformanceTool-algorithm/`

Apply the patches in numeric order to the source tree extracted from the RAR:

1. `01-core.patch`
2. `02-car-data.patch`
3. `03-analyzer.patch`
4. `04-maximizer.patch`
5. `05-ui-diagrams.patch`

The patches include the new source files (`GameMath.*`, `NFSWCarData.*`, and the car-data include blocks) as well as modifications to the existing Qt classes.

## Build note

The integration was checked structurally against the supplied Qt source. A native qmake/Qt build was not executed in the ChatGPT environment because Qt/qmake is not installed there.
