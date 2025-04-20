#ifndef ACTIONS_H
#define ACTIONS_H
#include "fsm_logic.h"

/* Input Actions */
ActionStatus actionMainMenu(AppContext* ctx, void* param);
ActionStatus actionFilePath(AppContext* ctx, void* param);
ActionStatus actionConfigMenu(AppContext* ctx, void* param);
ActionStatus actionResolutionConfig(AppContext* ctx, void* param);
ActionStatus actionColorModeConfig(AppContext* ctx, void* param);
ActionStatus actionPreviewChoice(AppContext* ctx, void* param);
ActionStatus actionFinalOutputChoice(AppContext* ctx, void* param);
ActionStatus actionOutputFileName(AppContext* ctx, void* param);

/* Check/Validation Actions */
ActionStatus actionValidateFilePath(AppContext* ctx, void* param);

/* Transform Actions */
ActionStatus actionParseBitmap(AppContext* ctx, void* param);
ActionStatus actionConvertToGrayscale(AppContext* ctx, void* param);
ActionStatus actionDetectEdges(AppContext* ctx, void* param);
ActionStatus actionChunkImage(AppContext* ctx, void* param);

/* Map Actions */
ActionStatus actionMapToAscii(AppContext* ctx, void* param);

/* Display Actions */
ActionStatus actionDisplayPreview(AppContext* ctx, void* param);
ActionStatus actionDisplayInTerminal(AppContext* ctx, void* param);

/* Output Actions */
ActionStatus actionSaveToFile(AppContext* ctx, void* param);

/* Helper Functions */
void clearInputBuffer();

#endif 