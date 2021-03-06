//
// Copyright 2020 The Project Oak Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

import React from 'react';
import { OakApplicationState } from '~/components/Root';

type ApplicationStateOverviewProps = {
  applicationState: OakApplicationState;
};

export default function ApplicationStateOverview({
  applicationState,
}: ApplicationStateOverviewProps) {
  return (
    <section>
      <strong>Application State</strong>
      <p>{applicationState.nodeInfos.size} Nodes:</p>
      <ul>
        {[...applicationState.nodeInfos.entries()].map(([id, nodeInfo]) => (
          <li key={id}>
            <dl>
              <dt>ID:</dt>
              <dd>{id}</dd>
              <dt>Name:</dt>
              <dd>{nodeInfo.name}</dd>
              <dt>Labels:</dt>
              <dd>{JSON.stringify(nodeInfo.label)}</dd>
              <dt>Handles:</dt>
              <dd>
                <ul>
                  {[...nodeInfo.abiHandles.entries()].map(
                    ([handle, channelHalf]) => (
                      <li key={handle}>
                        {handle} pointing to channel {channelHalf.channelId}{' '}
                        {channelHalf.direction}
                      </li>
                    )
                  )}
                </ul>
              </dd>
            </dl>
          </li>
        ))}
      </ul>
    </section>
  );
}
